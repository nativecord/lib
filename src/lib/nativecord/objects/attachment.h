 #pragma once

#include "nativecord/util/macros.h"

#include <stdint.h>
#include <string>

#include <nlohmann/json.hpp>

enum attachmentFlags
{
    ATTACHMENT_IS_REMIX = 1 << 2
};

struct Attachment
{
    public:
        uint64_t attachment;
        std::string filename;
        std::string descrption; // optional
        std::string content_type; // optional
        int size;
        std::string url;
        std::string proxy_url;
        int width = -1;          // optional
        int height = -1;          // optional
        bool ephemeral; // optional
        float duration_secs = -1; // optional
        std::string waveform; // optional
        attachmentFlags flags; // optional
};

NC_EXPORT inline void to_json(nlohmann::json& js, const Attachment& obj)
{
    NC_SERIALIZE(attachment);
    NC_SERIALIZE(filename);
    NC_SERIALIZE(descrption);
    NC_SERIALIZE(content_type);
    NC_SERIALIZE(size);
    NC_SERIALIZE(url);
    NC_SERIALIZE(proxy_url);
    NC_SERIALIZE(height);
    NC_SERIALIZE(width);
    NC_SERIALIZE(ephemeral);
    NC_SERIALIZE(duration_secs);
    NC_SERIALIZE(waveform);
    NC_SERIALIZE(flags);
    
    NC_SERIALIZE_CLEANUP(js);
}

NC_EXPORT inline void from_json(const nlohmann::json& js, Attachment& obj)
{
    NC_DESERIALIZE(attachment);
    NC_DESERIALIZE(filename);
    NC_DESERIALIZE(descrption);
    NC_DESERIALIZE(content_type);
    NC_DESERIALIZE(size);
    NC_DESERIALIZE(url);
    NC_DESERIALIZE(proxy_url);
    NC_DESERIALIZE(height);
    NC_DESERIALIZE(width);
    NC_DESERIALIZE(ephemeral);
    NC_DESERIALIZE(duration_secs);
    NC_DESERIALIZE(waveform);
    NC_DESERIALIZE(flags);
}