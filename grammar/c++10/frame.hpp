#pragma once
#include <variant>

#include "data.hpp"

namespace frame
{
    struct text
    {
        data::text * current_text;
    };

    struct line
    {
        data::line * current_line;
    };

    struct message
    {
        data::message * message;
    };

    using any = std::variant<text, line, message>;
}
