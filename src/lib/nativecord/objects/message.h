#pragma once

#include "nativecord/objects/attachment.h"
#include "nativecord/objects/channel.h"
#include "nativecord/objects/embed.h"
#include "nativecord/objects/role.h"
#include "nativecord/objects/user.h"
#include "nativecord/util/jsonutils.h"

// #include <functional>
#include <stdint.h>
#include <vector>

/*
    TO-DO
        implement self references
        (Message::referenced_message, MessageInteractionMetadata::triggering_interaction_metadata)
*/

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
NC_JSON_DECLFUNCS(ChannelMention, id, guild_id, name, type);

struct MessageActivity
{
        int type;
        std::optional<std::string> party_id;
};
NC_JSON_DECLFUNCS(MessageActivity, type, party_id);

struct MessageReference
{
        std::optional<uint64_t> message_id;
        std::optional<uint64_t> channel_id;
        std::optional<uint64_t> guild_id;
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
        // std::reference_wrapper<const MessageInteractionMetadata> triggering_interaction_metadata;
};
NC_JSON_DECLFUNCS(MessageInteractionMetadata, id, type, user, original_response_message_id, interacted_message_id);

struct Emoji
{
        std::optional<uint64_t> id;
        std::optional<std::string> name;
        std::optional<std::vector<Role>> roles;
        std::optional<User> user;
        std::optional<bool> require_colons;
        std::optional<bool> managed;
        std::optional<bool> animated;
        std::optional<bool> available;
};
NC_JSON_DECLFUNCS(Emoji, id, name, roles, user, require_colons, managed, animated, available);

struct ReactionCountDetails
{
        int burst;
        int normal;
};
NC_JSON_DECLFUNCS(ReactionCountDetails, burst, normal);

struct Reaction
{
        int count;
        ReactionCountDetails count_details;
        bool me;
        bool me_burst;
        Emoji emoji; // partial
        std::vector<int> burst_colors;
};
NC_JSON_DECLFUNCS(Reaction, count, count_details, me, me_burst, emoji, burst_colors);

struct StickerItem
{
        uint64_t id;
        std::string name;
        int format_type;
};
NC_JSON_DECLFUNCS(StickerItem, id, name, format_type);

struct RoleSubscriptionData
{
        uint64_t role_subscription_listing_id;
        std::string tier_name;
        int total_months_subscribed;
        bool is_renewal;
};
NC_JSON_DECLFUNCS(RoleSubscriptionData, role_subscription_listing_id, tier_name, total_months_subscribed, is_renewal);

struct Resolved
{
        std::optional<std::vector<uint64_t>> users;
        std::optional<std::vector<uint64_t>> members;
        std::optional<std::vector<uint64_t>> roles;
        std::optional<std::vector<uint64_t>> channels;
        std::optional<std::vector<uint64_t>> messages;
        std::optional<std::vector<uint64_t>> attachments;
};
NC_JSON_DECLFUNCS(Resolved, users, members, roles, channels, messages, attachments);

struct PollMediaObject
{
        std::optional<std::string> text;
        std::optional<Emoji> emoji; // partial
};
NC_JSON_DECLFUNCS(PollMediaObject, text, emoji);

struct PollAnswerObject
{
        std::optional<int> answer_id;
        PollMediaObject poll_media;
};
NC_JSON_DECLFUNCS(PollAnswerObject, answer_id, poll_media);

struct PollCreateRequest
{
        PollMediaObject question;
        std::vector<PollAnswerObject> answers;
        int duration;
        bool allow_multiselect;
        std::optional<int> layout_type;
};
NC_JSON_DECLFUNCS(PollCreateRequest, question, answers, duration, allow_multiselect, layout_type);

struct Message
{
        std::string id;
        std::string channel_id;
        std::string webhook_id;
        User author;
        std::string content;
        std::string timestamp;
        std::optional<std::string> edited_timestamp;
        bool tts;
        bool mention_everyone;
        bool pinned;
        std::vector<User> mentions;
        std::optional<std::vector<ChannelMention>> mention_channels;
        std::vector<Role> mention_roles;
        std::optional<std::vector<Attachment>> attachments;
        std::vector<Embed> embeds;
        std::optional<std::vector<Reaction>> reactions;
        std::optional<std::string> nonce;
        int type;
        // Application application;
        uint64_t application_id;
        std::optional<MessageReference> message_reference;
        std::optional<messageFlags> flags;
        std::optional<std::reference_wrapper<Message>> referenced_message;
        MessageInteractionMetadata interaction_metadata;
        // MessageInteraction interaction; // deprecated
        std::optional<Channel> thread;
        // MessageComponents components; // optional
        std::optional<std::vector<StickerItem>> sticker_items;
        // std::optional<std::vector<Sticker>> stickers; // deprecated
        std::optional<int> position;
        std::optional<RoleSubscriptionData> role_subscription_data;
        std::optional<Resolved> resolved;
        std::optional<PollCreateRequest> poll;
};
NC_JSON_DECLFUNCS(Message, id, channel_id, webhook_id, author, content, timestamp, edited_timestamp, tts,
                  mention_everyone, pinned, mentions, mention_channels, mention_roles, attachments, embeds, reactions,
                  nonce, type, application_id, message_reference, flags, /*referenced_message,*/
                  interaction_metadata, thread, sticker_items, position, role_subscription_data, resolved, poll)
