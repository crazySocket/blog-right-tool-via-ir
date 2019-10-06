#pragma once

#include <string>
#include <variant>

namespace symbols
{
    struct S {};
    struct Message {};
    struct Line {};

    struct Text {};

    struct Key : public Text {};
    struct Value : public Text {};
    struct Character
    {
        char ch;
    };

    using all = std::variant<S, Message, Line, Key, Value, Character, Text>;
}

std::string to_string(const symbols::S &)
{
    return "S";
}
std::string to_string(const symbols::Message &)
{
    return "Message";
}
std::string to_string(const symbols::Line &)
{
    return "Line";
}
std::string to_string(const symbols::Key &)
{
    return "Key";
}
std::string to_string(const symbols::Value &)
{
    return "Value";
}
std::string to_string(const symbols::Character & s)
{
    char buffer[20];
    sprintf(buffer, "Character: '%c'", s.ch);
    return std::string(buffer);
}
std::string to_string(const symbols::Text &)
{
    return "Text";
}

std::string to_string(const symbols::all & variant)
{
    return std::visit(
        [](auto symbol)
        {
            return to_string(symbol);
        },
        variant
    );
}
