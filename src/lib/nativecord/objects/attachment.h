#pragma once

#include "nativecord/util/jsonutils.h"

#include <stdint.h>
#include <string>

enum attachmentFlags
{
    ATTACHMENT_IS_REMIX = 1 << 2
};

struct Attachment
{
        uint64_t attachment;
        std::string filename;
        std::optional<std::string> description;
        std::optional<std::string> content_type;
        int size;
        std::string url;
        std::string proxy_url;
        std::optional<int> width;
        std::optional<int> height;
        std::optional<bool> ephemeral;
        std::optional<float> duration_secs;
        std::optional<std::string> waveform;
        std::optional<attachmentFlags> flags;
};
NC_JSON_DECLFUNCS(Attachment, attachment, filename, description, content_type, size, url, proxy_url, width, height,
                  ephemeral, duration_secs, waveform, flags);
