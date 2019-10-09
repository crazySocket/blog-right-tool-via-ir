#pragma once
#include <variant>

namespace symbol
{
    struct message {};
    struct line {};
    struct key {};
    struct value {};
    struct character
    {
        char ch;
    };
    struct not_delimeter {};
    struct not_delimeter_more {};

    using any = std::variant<message, line, key, value, character, not_delimeter, not_delimeter_more>;
}
