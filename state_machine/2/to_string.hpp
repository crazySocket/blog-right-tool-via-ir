#pragma once
#include <string>

#include "state.hpp"
#include "storage.hpp"

#define TO_STRING_IMPL(x) inline std::string to_string(x) { return #x; }
TO_STRING_IMPL(state::A)
TO_STRING_IMPL(state::B)
TO_STRING_IMPL(state::C)
#undef TO_STRING_IMPL

template<typename M>
std::string to_string(M & m)
{
    return visit(
        [](auto s)
        {
            return to_string(s);
        },
        m
    );
}
