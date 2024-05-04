#include "objects.h"
#include "user.h"
#include "channel.h"

enum messageFlags : int32_t
{
    MESSAGEFLAG_CROSSPOSTED = 1 << 0,
    MESSAGEFLAG_IS_CROSSPOST = 1 << 1,
    MESSAGEFLAG_SUPRESS_EMBEDS = 1 << 2,
    MESSAGEFLAG_SOURCE_MESSAGE_DELETED = 1 << 3,
    MESSAGEFLAG_URGENT = 1 << 4,
    MESSAGEFLAG_HAS_THREAD = 1 << 5,
    MESSAGEFLAG_EPHEMERAL = 1 << 6,
    MESSAGEFLAG_LOADING = 1 << 7,
    MESSAGEFLAG_FAILED_TO_MENTION_SOME_ROLES_IN_THREAD = 1 << 8,
    MESSAGEFLAG_SUPPRESS_NOTIFICATIONS = 1 << 12,
    MESSAGEFLAG_IS_VOICE_MESSAGE = 1 << 13
};

enum messageTypes : int32_t
{
    MESSAGE_DEFAULT = 0,                                       // deletable: true
    MESSAGE_RECIPIENT_ADD = 1,                                 // deletable: false
    MESSAGE_RECIPIENT_REMOVE = 2,                              // deletable: false
    MESSAGE_CALL = 3,                                          // deletable: false
    MESSAGE_CHANNEL_NAME_CHANGE = 4,                           // deletable: false
    MESSAGE_CHANNEL_ICON_CHANGE = 5,                           // deletable: false
    MESSAGE_CHANNEL_PINNED_MESSAGE = 6,                        // deletable: true
    MESSAGE_USER_JOIN = 7,                                     // deletable: true
    MESSAGE_GUILD_BOOST = 8,                                   // deletable: true
    MESSAGE_GUILD_BOOST_TIER_1 = 9,                            // deletable: true
    MESSAGE_GUILD_BOOST_TIER_2 = 10,                           // deletable: true
    MESSAGE_GUILD_BOOST_TIER_3 = 11,                           // deletable: true
    MESSAGE_CHANNEL_FOLLOW_ADD = 12,                           // deletable: true
    MESSAGE_GUILD_DISCOVERY_DISQUALIFIED = 14,                 // deletable: false
    MESSAGE_GUILD_DISCOVERY_REQUALIFIED = 15,                  // deletable: false
    MESSAGE_GUILD_DISCOVERY_GRACE_PERIOD_INITIAL_WARNING = 16, // deletable: false
    MESSAGE_GUILD_DISCOVERY_GRACE_PERIOD_FINAL_WARNING = 17,   // deletable: false
    MESSAGE_THREAD_CREATED = 18,                               // deletable: true
    MESSAGE_REPLY = 19,                                        // deletable: true
    MESSAGE_CHAT_INPUT_COMMAND = 20,                           // deletable: true
    MESSAGE_THREAD_STARTER_MESSAGE = 21,                       // deletable: false
    MESSAGE_GUILD_INVITE_REMINDER = 22,                        // deletable: true
    MESSAGE_CONTEXT_MENU_COMMAND = 23,                         // deletable: true
    MESSAGE_AUTO_MODERATION_ACTION = 24,                       // deletable: true
    MESSAGE_ROLE_SUBSCRIPTION_PURCHASE = 25,                   // deletable: true
    MESSAGE_INTERACTION_PREMIUM_UPSELL = 26,                   // deletable: true
    MESSAGE_STAGE_START = 27,                                  // deletable: true
    MESSAGE_STAGE_END = 28,                                    // deletable: true
    MESSAGE_STAGE_SPEAKER = 29,                                // deletable: true
    MESSAGE_STAGE_TOPIC = 31,                                  // deletable: true
    MESSAGE_GUILD_APPLICATION_PREMIUM_SUBSCRIPTION = 32        // deletable: true
};

enum messageInteractionType
{
    INTERACTION_PING = 1,
    INTERACTION_APPLICATION_COMMAND,
    INTERACTION_MESSAGE_COMPONENT = 3,
    INTERACTION_APPLICATION_COMMAND_AUTOCOMPLETE = 4,
    INTERACTION_MODAL_SUBMIT = 5
};

class ChannelMention
{
    public:
        snowflake id;
        snowflake guild_id;
        int type;
        std::string name;
};
NC_JSON_DECLFUNCS(ChannelMention, id, guild_id, type, name);

class MessageReference
{
    public:
        std::optional<snowflake> message_id;
        std::optional<snowflake> channel_id;
        std::optional<snowflake> guild_id;
        std::optional<bool> fail_if_not_exists;
};
NC_JSON_DECLFUNCS(MessageReference, message_id, channel_id, guild_id, fail_if_not_exists);

struct MessageInteractionMetadata
{
        uint64_t id;
        messageInteractionType type;
        User user;
        std::vector<int> authorizing_integration_owners;
        uint64_t original_response_message_id;
        uint64_t interacted_message_id;
        // MessageInteractionMetadata triggering_interaction_metadata;
};
NC_JSON_DECLFUNCS(MessageInteractionMetadata, id, type, user, original_response_message_id, interacted_message_id);

struct StickerItem
{
        uint64_t id;
        std::string name;
        int format_type;
};
NC_JSON_DECLFUNCS(StickerItem, id, name, format_type);

class Message
{
    public:
        Message();
        ~Message();

        std::optional<snowflake> id; // always received, not sent
        std::optional<snowflake> channel_id; // always received, not sent
        std::optional<snowflake> webhook_id;
        std::optional<User> author; // always received, not sent
        std::string content;
        std::optional<std::string> timestamp; // always received, not sent
        std::optional<std::string> edited_timestamp;
        bool tts = false;
        bool mention_everyone = false;
        std::optional<bool> pinned; // always received, not sent
        std::vector<User> mentions = {};
        std::vector<snowflake> mention_roles = {};
        std::optional<std::vector<ChannelMention>> mention_channels;
        //std::optional<std::vector<Attachment>> attachments;
        //std::vector<Embed> embeds;
        //std::optional<std::vector<Reaction>> reactions;
        std::optional<std::string> nonce;
        std::optional<messageTypes> type; // always received, not sent
        // Application application;
        std::optional<snowflake> application_id;
        std::optional<MessageReference> message_reference;
        std::optional<messageFlags> flags;
        std::optional<Message*> referenced_message;
        MessageInteractionMetadata interaction_metadata;
        // MessageInteraction interaction; // deprecated
        std::optional<Channel> thread;
        // MessageComponents components;
        std::optional<std::vector<StickerItem>> sticker_items;
        // std::optional<std::vector<Sticker>> stickers; // deprecated
        std::optional<int> position;
        //std::optional<RoleSubscriptionData> role_subscription_data;
        //std::optional<Resolved> resolved;
        //std::optional<PollCreateRequest> poll;
};

void to_json(nlohmann::json& nlohmann_json_j, const Message& nlohmann_json_t);
void from_json(const nlohmann::json& nlohmann_json_j, Message& nlohmann_json_t);