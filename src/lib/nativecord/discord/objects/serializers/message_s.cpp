#include "message_s.h"
#include "channel_s.h"
#include "embed_s.h"
#include "user_s.h"

#include "nativecord/discord/objects/message.h"

namespace nativecord
{
    NC_JSON_FUNCS(ChannelMention, id, guild_id, type, name);
    NC_JSON_FUNCS(MessageReference, message_id, channel_id, guild_id, fail_if_not_exists);
    NC_JSON_FUNCS(MessageInteractionMetadata, id, type, user, original_response_message_id, interacted_message_id);
    NC_JSON_FUNCS(StickerItem, id, name, format_type);

    NC_EXPORT void to_json(nlohmann::json& nlohmann_json_j, const Message& nlohmann_json_t)
    {
        NLOHMANN_JSON_PASTE(NC_TO_JSON, id, channel_id, webhook_id, guild_id, author, content, timestamp,
                            edited_timestamp, tts, mention_everyone, pinned, mentions, mention_roles, mention_channels,
                            embeds, nonce, type, application_id, message_reference, flags, interaction_metadata, thread,
                            sticker_items, position);
    }
    NC_EXPORT void from_json(const nlohmann::json& nlohmann_json_j, Message& nlohmann_json_t)
    {
        if (nlohmann_json_j.contains("referenced_message"))
        {
            Message* msg = new Message();
            *msg = nlohmann_json_j["referenced_message"];
            nlohmann_json_t.referenced_message = msg;
        }

        NLOHMANN_JSON_PASTE(NC_FROM_JSON, id, channel_id, webhook_id, guild_id, author, content, timestamp,
                            edited_timestamp, tts, mention_everyone, pinned, mentions, mention_roles, mention_channels,
                            embeds, nonce, type, application_id, message_reference, flags, interaction_metadata, thread,
                            sticker_items, position);
    }
}
