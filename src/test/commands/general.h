#pragma once

#include "commandmanager.h"

#include <format>
#include <vector>

std::vector<std::pair<std::string, std::string>> rules = {
    {"Be nice", "Treat everyone with respect. Harassment, hate speech, discrimination, or bullying of any kind will "
                "not be tolerated."},
    {"No weird stuff", "Keep the server appropriate for all ages. No explicit or adult content, including images, "
                       "videos, or discussions."},
    {"Stay on topic", "Keep discussions relevant to the channels they're in. Off-topic conversations should be moved "
                      "to the appropriate channel or kept to a minimum."},
    {"Don't spam",
     "Avoid excessive messaging, emojis, or repeated messages. Spamming disrupts conversations and clutters the chat. "
     "And don't promote other Discord servers, products, or services without permission from the server admins."},
    {"English please", "Keep communication in English to ensure everyone can understand and participate."},
    {"No selfbots", "Usage or discussion of selfbots is strictly prohibited."},
    {"Think before pinging", "Don't ping someone (specially for help) unless you really have to."},
    {"No nerds!!!!",
     "If you feel there's something missing here (rules, loopholes, whatever), please let us know instead of abusing "
     "it. Being a smartass will only result on an instant permanent ban."},
    {"Have a name that can be mentioned", "(or set your nick to one.)"},
    {"Do not impersonate users", "(Unless it's blatantly obvious, for comedic purposes and we find it funny)"},
    {"Consider privacy", "Do not share personal information about yourself or others without their consent. This "
                         "includes real names, addresses, phone numbers, and any other sensitive information."},
    {"No backseat moderation", "Leave moderation to the server staff. If you notice a rule violation, report it to a "
                               "moderator instead of trying to enforce the rules yourself."},
    {"Use common sense",
     "If you're having doubts on whether something is allowed or not, then it probably isn't, so don't do that."},
    {"Follow Discord Terms of Service", "Abide by the Discord Community Guidelines and Terms of Service."}};

Command* pingCommand = new Command(
    "ping", "pong", [](Client*, std::shared_ptr<Channel> channel, Message* msg, std::vector<std::string>*) {
        Message response{};
        response.content = "pong";
        response.message_reference =
            std::make_optional<MessageReference>({msg->id, std::nullopt, std::nullopt, std::nullopt});
        channel->sendMessage(response);
    });

Command* rulesCommand = new Command(
    "rules", "Sends a message containing the server rules.",
    [](Client*, std::shared_ptr<Channel> channel, Message* msg, std::vector<std::string>*) {
        Embed responseEmbed;
        responseEmbed.title = "Server rules";
        responseEmbed.setColor(60, 0, 60);

        int idx = 1;
        for (auto rule : rules)
        {
            responseEmbed.addField(std::format("{} - {}", idx, rule.first), rule.second);
            idx++;
        }

        Message response{};
        response.addEmbed(responseEmbed);
        channel->sendMessage(response);
    },
    true);

CommandCategory* generalCategory = new CommandCategory("General", {pingCommand, rulesCommand});
