#include "rules.hpp"

#include <iostream>
using std::cout;
using std::endl;

template<>
parser::result::any parser::parse(symbol::not_delimeter, frame::text & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    if(isalnum(next_char) || next_char == ' ')
    {
        frame.current_text->content.push_back(next_char);
        return {result::finished{.consume = true, .frame = false}};
    }
    else
    {
        return {result::mismatch{}};
    }
}

template<>
parser::result::any parser::parse(symbol::not_delimeter_more, frame::text & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    if(isalnum(next_char) || next_char == ' ')
    {
        frame.current_text->content.push_back(next_char);
        return {result::consumed{}};
    }
    else
    {
        return {result::finished{.consume = false, .frame = true}};
    }
}

template<>
parser::result::any parser::parse(symbol::value, frame::line & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return
    {
        result::change_frame
        {
            .frame = frame::text{&frame.current_line->value}
        }
    };
}

template<>
parser::result::any parser::parse(symbol::character symbol, frame::line & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    switch (symbol.ch)
    {
        case ':':
            return {result::finished{.consume = true, .frame = false}};
        case '\n':
            return {result::finished{.consume = true, .frame = true}};
        default:
            return {result::mismatch{}};
    }
}

template<>
parser::result::any parser::parse(symbol::key, frame::line & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return
    {
        result::change_frame
        {
            .frame = frame::text{&frame.current_line->key}
        }
    };
}

template<>
parser::result::any parser::parse(symbol::line, frame::line & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return
    {
        result::append_symbols
        {
            .symbols = {symbol::key{}, symbol::character{':'}, symbol::value{}, symbol::character{'\n'}}
        }
    };
}

template<>
parser::result::any parser::parse(symbol::line, frame::message & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    frame.message->lines.push_back({});
    return
    {
        result::change_frame
        {
            .frame = frame::line{&frame.message->lines.back()}
        }
    };
}

template<>
parser::result::any parser::parse(symbol::message, frame::message & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return
    {
        result::append_symbols
        {
            .symbols = {symbol::line{}, symbol::message{}}
        }
    };
}

template<>
parser::result::any parser::parse(symbol::message, frame::message & frame)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return {result::finished{.consume = false, .frame = false}};
}
