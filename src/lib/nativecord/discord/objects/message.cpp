#include "message.h"

Message::Message() {}

Message::~Message()
{
    if (referenced_message != std::nullopt)
        delete referenced_message.value();
}

void to_json(nlohmann::json& nlohmann_json_j, const Message& nlohmann_json_t)
{
    NLOHMANN_JSON_PASTE(NC_TO_JSON, id, channel_id, webhook_id, author, content, timestamp, edited_timestamp, tts,
                        mention_everyone, pinned, mentions, mention_roles, mention_channels, nonce, type,
                        application_id, message_reference, flags, interaction_metadata, thread, sticker_items,
                        position);
}

void from_json(const nlohmann::json& nlohmann_json_j, Message& nlohmann_json_t)
{
    if (nlohmann_json_j.contains("referenced_message"))
    {
        Message* msg = new Message();
        *msg = nlohmann_json_j["referenced_message"];
        nlohmann_json_t.referenced_message = msg;
    }
    NLOHMANN_JSON_PASTE(NC_FROM_JSON, id, channel_id, webhook_id, author, content, timestamp, edited_timestamp, tts,
                        mention_everyone, pinned, mentions, mention_roles, mention_channels, nonce, type,
                        application_id, message_reference, flags, interaction_metadata, thread, sticker_items,
                        position);
}
