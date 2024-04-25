#pragma once

/*
    https://github.com/discord/discord-api-docs/blob/main/docs/topics/Opcodes_and_Status_Codes.md
*/

enum gatewayCode
{
    GATEWAY_INVALID = -1,
    GATEWAY_DISPATCH = 0,
    GATEWAY_HEARTBEAT = 1,
    GATEWAY_IDENTIFY = 2,
    GATEWAY_PRESENCE_UPDATE = 3,
    GATEWAY_VOICE_STATE_UPDATE = 4,
    GATEWAY_RESUME = 6,
    GATEWAY_RECONNECT = 7,
    GATEWAY_REQUEST_GUILD_MEMBERS = 8,
    GATEWAY_INVALID_SESSION = 9,
    GATEWAY_HELLO = 10,
    GATEWAY_HEARTBEAT_ACK = 11
};

enum gatewayVoiceCode
{
    VOICE_IDENTIFY = 0,
    VOICE_SELECT_PROTOCOL = 1,
    VOICE_READY = 2,
    VOICE_HEARTBEAT = 3,
    VOICE_SESSION_DESCRIPTION = 4,
    VOICE_SPEAKING = 5,
    VOICE_HEARTBEAT_ACK = 6,
    VOICE_RESUME = 7,
    VOICE_HELLO = 8,
    VOICE_RESUMED = 9,
    VOICE_CLIENT_DISCONNECT = 13
};

enum gatewayCloseCode
{
    CLOSE_UNKNOWN_ERROR = 4000,
    CLOSE_UNKNOWN_OPCODE = 4001,
    CLOSE_DECODE_ERROR = 4002,
    CLOSE_NOT_AUTHENTICATED = 4003,
    CLOSE_AUTHENTICATION_FAILED = 4004,
    CLOSE_ALREADY_AUTHENTICATED = 4005,
    CLOSE_INVALID_SEQ = 4007,
    CLOSE_RATE_LIMITED = 4008,
    CLOSE_SESSION_TIMED_OUT = 4009,
    CLOSE_INVALID_SHARD = 4010,
    CLOSE_SHARDING_REQUIRED = 4011,
    CLOSE_INVALID_API_VERSION = 4012,
    CLOSE_INVALID_INTENTS = 4013,
    CLOSE_DISALLOWED_INTENTS = 4014
};

enum gatewayIntents : int
{
    INTENT_GUILDS = 1 << 0,
    INTENT_GUILD_MEMBERS = 1 << 1,
    INTENT_GUILD_BANS = 1 << 2,
    INTENT_GUILD_EMOJIS = 1 << 3,
    INTENT_GUILD_INTEGRATIONS = 1 << 4,
    INTENT_GUILD_WEBHOOKS = 1 << 5,
    INTENT_GUILD_INVITES = 1 << 6,
    INTENT_GUILD_VOICE_STATES = 1 << 7,
    INTENT_GUILD_PRESENCES = 1 << 8,
    INTENT_GUILD_MESSAGES = 1 << 9,
    INTENT_GUILD_MESSAGE_REACTIONS = 1 << 10,
    INTENT_GUILD_MESSAGE_TYPING = 1 << 11,
    INTENT_GUILD_SCHEDULED_EVENTS = 1 << 16,
    INTENT_GUILD_MESSAGE_POLLS = 1 << 24,

    INTENT_DIRECT_MESSAGES = 1 << 12,
    INTENT_DIRECT_MESSAGE_REACTIONS = 1 << 13,
    INTENT_DIRECT_MESSAGE_TYPING = 1 << 14,
    INTENT_DIRECT_MESSAGE_POLLS = 1 << 25,

    INTENT_MESSAGE_CONTENT = 1 << 15,

    INTENT_AUTO_MODERATION_CONFIGURATION = 1 << 20,
    INTENT_AUTO_MODERATION_EXECUTION = 1 << 21
};