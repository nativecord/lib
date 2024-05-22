#include "http.h"

#include <hv/HttpClient.h>
#include <hv/requests.h>

static hv::HttpClient* client = new hv::HttpClient();

std::pair<int, std::string> nativecord::http::req(std::string url, http_method method, std::optional<std::string> body,
                                                  std::unordered_map<std::string, std::string>* headers,
                                                  std::string userAgent)
{
    HttpRequest req;
    req.method = method;
    req.url = url;
    req.timeout = 10;
    req.headers["User-Agent"] = userAgent;

    if (body != std::nullopt)
        req.body = body.value();

    if (headers)
        for (auto header : *headers)
            req.headers[header.first] = header.second;

    HttpResponse res;
    client->send(&req, &res);
    return std::make_pair(res.status_code, res.body);
}
