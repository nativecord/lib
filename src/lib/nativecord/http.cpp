#include "http.h"

#include "nativecord/util/macros.h"

#include <libwebsockets.h>

#include <stdexcept>
#include <vector>

lws_context* context;

struct requestInfo
{
        const char* _send = nullptr;
        size_t _sendSize = 0;

        std::vector<char> _recv = std::vector<char>();

        std::string _userAgent;
        std::unordered_map<std::string, std::string> _headers;

        bool _ready;
        bool _err;
};

#define HTTP_ERROR_IF(cond)                                                                                            \
    if (cond)                                                                                                          \
    {                                                                                                                  \
        req->_err = true;                                                                                              \
        lws_cancel_service(lws_get_context(wsi));                                                                      \
        return -1;                                                                                                     \
    }

int httpCallback(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len)
{
    if (!user)
        return 0;

    requestInfo* req = reinterpret_cast<requestInfo*>(user);

    switch (reason)
    {
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
            {
                unsigned char** start = reinterpret_cast<unsigned char**>(in);
                unsigned char* end = *start + len - 1;

                int headerResult = 0;
                headerResult = lws_add_http_header_by_token(wsi, WSI_TOKEN_HTTP_USER_AGENT,
                                                            reinterpret_cast<const unsigned char*>(NC_USERAGENT),
                                                            static_cast<int>(strlen(NC_USERAGENT)), start, end);

                HTTP_ERROR_IF(headerResult)

                for (auto header : req->_headers)
                {
                    headerResult =
                        lws_add_http_header_by_name(wsi, reinterpret_cast<const unsigned char*>(header.first.c_str()),
                                                    reinterpret_cast<const unsigned char*>(header.second.c_str()),
                                                    static_cast<int>(header.second.length()), start, end);
                    HTTP_ERROR_IF(headerResult)
                }

                if (lws_http_is_redirected_to_get(wsi))
                    break;

                if (req->_send != nullptr && req->_sendSize > 0)
                {
                    char lengthBuffer[20];
                    snprintf(lengthBuffer, 20, "%zu", req->_sendSize);

                    // so much casting...
                    headerResult = lws_add_http_header_by_token(
                        wsi, WSI_TOKEN_HTTP_CONTENT_LENGTH,
                        reinterpret_cast<const unsigned char*>(const_cast<const char*>(lengthBuffer)),
                        static_cast<int>(strlen(lengthBuffer)), start, end);

                    HTTP_ERROR_IF(headerResult)

                    lws_client_http_body_pending(wsi, 1);
                    lws_callback_on_writable(wsi);
                }
                break;
            }
        case LWS_CALLBACK_CLIENT_HTTP_WRITEABLE:
            {
                if (lws_http_is_redirected_to_get(wsi))
                    break;

                size_t size = req->_sendSize;
                unsigned char* writeBuff = reinterpret_cast<unsigned char*>(malloc(LWS_PRE + size));
                HTTP_ERROR_IF(!writeBuff)
                auto start = writeBuff + LWS_PRE;
                memcpy(start, req->_send, size);

                int res = lws_write(wsi, start, size, LWS_WRITE_HTTP);
                free(writeBuff);

                HTTP_ERROR_IF(res < 0 || res < static_cast<int>(size))

                lws_client_http_body_pending(wsi, 0);

                return 0;
            }
        case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
            {
                char* start = reinterpret_cast<char*>(in);
                char* end = start + len;
                req->_recv.insert(req->_recv.end(), start, end);
                return 0;
            }
        case LWS_CALLBACK_RECEIVE_CLIENT_HTTP:
            {
                char* buf = reinterpret_cast<char*>(malloc(LWS_PRE + NC_HTTP_BUFFSIZE));
                HTTP_ERROR_IF(!buf)

                char* cur = buf + LWS_PRE;
                int bufLen = NC_HTTP_BUFFSIZE;
                if (lws_http_client_read(wsi, &cur, &bufLen) < 0)
                {

                    req->_err = true;
                    lws_cancel_service(lws_get_context(wsi));
                    free(buf);
                    return -1;
                }
                free(buf);
                return 0;
            }
        case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
            {
                req->_recv.push_back('\0');
                req->_ready = true;
                lws_cancel_service(lws_get_context(wsi));
                break;
            }
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            req->_err = true;
            lws_cancel_service(lws_get_context(wsi));
            return -1;
        default:
            break;
    }

    return lws_callback_http_dummy(wsi, reason, user, in, len);
}

static lws_protocols protocols[] = {{"http", httpCallback, 0, 0, 0, NULL, 0}, LWS_PROTOCOL_LIST_TERM};
static lws_extension extensions[] = {{0, 0, 0}};
void ensureContext()
{
    if (context)
        return;

    lws_set_log_level(0, 0);

    lws_context_creation_info cci{};
    cci.protocols = protocols;
    cci.extensions = extensions;
    cci.port = CONTEXT_PORT_NO_LISTEN;
    cci.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    cci.gid = -1;
    cci.uid = -1;
    cci.user = 0;

    context = lws_create_context(&cci);
    if (!context)
        throw new std::runtime_error("failed to create context for http request");
}

std::unique_ptr<char[]> nativecord::http::request(const char* url, const char* method, const char* data, size_t size,
                                                  std::unordered_map<std::string, std::string>* headers,
                                                  std::string userAgent)
{
    ensureContext();

    requestInfo req{};
    req._userAgent = userAgent;
    if (headers)
        req._headers = *headers;
    if (data && size)
    {
        req._send = data;
        req._sendSize = size;
    }

    lws_client_connect_info connInfo{};

    char* urlCopy = reinterpret_cast<char*>(malloc(strlen(url) + 1));
    if (!urlCopy)
        throw new std::runtime_error("failed to allocate memory for http uri");
    memcpy(urlCopy, url, strlen(url));
    urlCopy[strlen(url)] = '\0';
    if (lws_parse_uri(urlCopy, &connInfo.protocol, &connInfo.address, &connInfo.port, &connInfo.path))
    {
        free(urlCopy);
        throw new std::runtime_error("failed to parse uri");
    }

    lws* wsi;
    connInfo.pwsi = &wsi;
    connInfo.context = context;
    connInfo.host = connInfo.address;
    connInfo.origin = connInfo.address;
    connInfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_PIPELINE | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_ALLOW_INSECURE |
                              LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;
    connInfo.ietf_version_or_minus_one = -1;
    connInfo.protocol = protocols[0].name;
    connInfo.userdata = &req;
    connInfo.method = method;

    lws_client_connect_via_info(&connInfo);

    free(urlCopy);

    if (!wsi)
        throw new std::runtime_error("failed to connect");

    while (!req._ready && !req._err)
        lws_service(context, 0);

    if (req._err)
        return nullptr;

    size_t recvSize = req._recv.size();
    auto recvBuffer = std::unique_ptr<char[]>(new char[recvSize + 1]);
    memcpy(recvBuffer.get(), req._recv.data(), recvSize);

    return recvBuffer;
}