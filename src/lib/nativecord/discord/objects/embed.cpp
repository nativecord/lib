#include "embed.h"

void nativecord::Embed::addField(std::string name, std::string value, std::optional<bool> isInline)
{
    EmbedField field;
    field.name = name;
    field.value = value;
    field.inline_ = isInline;

    if (fields == std::nullopt)
        fields = std::vector<EmbedField>{};

    fields.value().push_back(field);
}

void nativecord::Embed::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    color = (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b);
}