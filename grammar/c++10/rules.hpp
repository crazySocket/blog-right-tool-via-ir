#pragma once
#include <iostream>

#include "frame.hpp"
#include "parser.hpp"
#include "symbol.hpp"

/*
 * S -> M
 * M -> L M
 * M -> e
 * L -> K : V \n
 * K -> T
 * V -> T
 * T -> not_delim not_delim_more
 * not_delim_more -> e
 */

#define PARSER_TABLE_ENTRY_WITHOUT_INPUT(S, F) template<> parser::result::any parser::parse(S, F&);
#define PARSER_TABLE_ENTRY_WITH_INPUT(S, F) template<> parser::result::any parser::parse(S, F&, char);

PARSER_TABLE_ENTRY_WITH_INPUT(symbol::not_delimeter, frame::text)
PARSER_TABLE_ENTRY_WITH_INPUT(symbol::not_delimeter_more, frame::text)

template<typename S>
parser::result::any parser::parse(S, frame::text & frame, char next_char)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return
    {
        result::append_symbols
        {
            .symbols = {symbol::not_delimeter{}, symbol::not_delimeter_more{}}
        }
    };
}

PARSER_TABLE_ENTRY_WITH_INPUT(symbol::value, frame::line)
PARSER_TABLE_ENTRY_WITH_INPUT(symbol::character, frame::line)
PARSER_TABLE_ENTRY_WITH_INPUT(symbol::key, frame::line)
PARSER_TABLE_ENTRY_WITH_INPUT(symbol::line, frame::line)
PARSER_TABLE_ENTRY_WITH_INPUT(symbol::line, frame::message)
PARSER_TABLE_ENTRY_WITH_INPUT(symbol::message, frame::message)

#undef PARSER_TABLE_ENTRY_WITHOUT_INPUT
#undef PARSER_TABLE_ENTRY_WITH_INPUT
