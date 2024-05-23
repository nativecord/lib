#include "embed.h"

using namespace nativecord;

void Embed::addField(std::string name, std::string value, std::optional<bool> isInline)
{
    EmbedField field;
    field.name = name;
    field.value = value;
    field.inline_ = isInline;

    if (fields == std::nullopt)
        fields = std::vector<EmbedField>{};

    fields.value().push_back(field);
}

void Embed::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    color = (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b);
}

void Embed::getColor(uint8_t& r, uint8_t& g, uint8_t& b)
{
    if (!color)
        return;
    r = static_cast<uint8_t>((color.value() >> 16) & 0xFF);
    g = static_cast<uint8_t>((color.value() >> 8) & 0xFF);
    b = static_cast<uint8_t>(color.value() & 0xFF);
}
