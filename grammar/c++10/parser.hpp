#pragma once
#include <iostream>
#include <list>
#include <variant>

#include "symbol.hpp"

namespace parser
{
    namespace result
    {
        struct mismatch {};
        struct consumed {};
        struct finished
        {
            bool consume;
            bool frame;
        };
        struct append_symbols
        {
            std::list<symbol::any> symbols;
        };
        struct change_frame
        {
            frame::any frame;
        };
        struct any : std::variant<mismatch, consumed, finished, append_symbols, change_frame> {};
    }

    template<typename S, typename F>
    result::any parse(S symbol, F & frame, char next_char)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return {result::mismatch{}};
    }

    template<typename S, typename F>
    result::any parse(S symbol, F & frame)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return {result::mismatch{}};
    }
}
