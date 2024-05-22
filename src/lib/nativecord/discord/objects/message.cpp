#include "message.h"

namespace nativecord
{
    Message::Message() {}

    Message::~Message()
    {
        if (referenced_message != std::nullopt)
            delete referenced_message.value();
    }
}
