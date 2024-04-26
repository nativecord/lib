#pragma once

#include "user.h"

#include <any>
#include <vector>

#include <nlohmann/json.hpp>

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

enum messageFlags
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

enum messageInteractionType
{
    INTERACTION_PING = 1,
    INTERACTION_APPLICATION_COMMAND,
    INTERACTION_MESSAGE_COMPONENT = 3,
    INTERACTION_APPLICATION_COMMAND_AUTOCOMPLETE = 4,
    INTERACTION_MODAL_SUBMIT = 5
};

struct ChannelMention
{
        uint64_t id;
        uint64_t guild_id;
        std::string name;
        int type;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ChannelMention, id, guild_id, name, type);

struct MessageActivity
{
        int type;
        std::string party_id; // optional
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MessageActivity, type, party_id);

struct MessageReference
{
        uint64_t message_id;     // optional
        uint64_t channel_id;     // optional
        uint64_t guild_id;       // optional
        bool fail_if_not_exists; // optional
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MessageReference, message_id, channel_id, guild_id, fail_if_not_exists);

struct MessageInteractionMetadata
{
        uint64_t id;
        messageInteractionType type;
        User user;
        // authorizing_integration_owners;
        uint64_t original_response_message_id;
        uint64_t interacted_message_id;
        // MessageInteractionMetadata triggering_interaction_metadata;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MessageInteractionMetadata, id, type, user,
                                                original_response_message_id, interacted_message_id);

struct Message
{
    public:
        std::string id;
        std::string channel_id;
        std::string webhook_id;
        User author;
        std::string content;
        std::string timestamp;
        std::string edited_timestamp; // optional
        bool tts;
        bool mention_everyone;
        bool pinned;

        std::vector<User> mentions;
        std::vector<ChannelMention> mention_channels; // optional
        // std::vector<Role> mention_roles;
        // std::vector<Attachment> attachments; // optional
        // std::vector<Embed> embeds;
        // std::vector<Reaction> reactions; // optional

        std::string nonce; // optional
        int type;

        // Application application;

        uint64_t application_id;
        MessageReference message_reference; // optional
        messageFlags flags;                 // optional

        // Message referenced_message; // optional

        MessageInteractionMetadata interaction_metadata;

        // MessageInteraction interaction; // deprecated
        // Channel thread; // optional
        // MessageComponents components; // optional
        // std::vector<MessageStickerItem> sticker_items; // optional
        // std::vector<Sticker> stickers; // optional, deprecated

        int position; // optional

        // RoleSubscriptionData role_subscription_data; // optional
        // Resolved resolved; // optional
        // Poll poll; // optional
};

NC_EXPORT inline void to_json(nlohmann::json& js, const Message& obj)
{
    NC_SERIALIZE(id);
    NC_SERIALIZE(channel_id);
    NC_SERIALIZE(webhook_id);
    NC_SERIALIZE(author);
    NC_SERIALIZE(content);
    NC_SERIALIZE(timestamp);
    NC_SERIALIZE(edited_timestamp);
    NC_SERIALIZE(tts);
    NC_SERIALIZE(mention_everyone);
    NC_SERIALIZE(pinned);
    NC_SERIALIZE(mentions);
    NC_SERIALIZE(mention_channels);
    NC_SERIALIZE(nonce);
    NC_SERIALIZE(type);
    NC_SERIALIZE(application_id);
    NC_SERIALIZE(message_reference);
    NC_SERIALIZE(flags);
    NC_SERIALIZE(interaction_metadata);
    NC_SERIALIZE(position);

    NC_SERIALIZE_CLEANUP(js);
}

NC_EXPORT inline void from_json(const nlohmann::json& js, Message& obj)
{
    std::string dbg = js.dump();
    dbg;

    NC_DESERIALIZE(id);
    NC_DESERIALIZE(channel_id);
    NC_DESERIALIZE(webhook_id);
    NC_DESERIALIZE(author);
    NC_DESERIALIZE(content);
    NC_DESERIALIZE(timestamp);
    NC_DESERIALIZE(edited_timestamp);
    NC_DESERIALIZE(tts);
    NC_DESERIALIZE(mention_everyone);
    NC_DESERIALIZE(pinned);
    NC_DESERIALIZE(mentions);
    NC_DESERIALIZE(mention_channels);
    NC_DESERIALIZE(nonce);
    NC_DESERIALIZE(type);
    NC_DESERIALIZE(application_id);
    NC_DESERIALIZE(message_reference);
    NC_DESERIALIZE(flags);
    NC_DESERIALIZE(interaction_metadata);
    NC_DESERIALIZE(position);
}
