#pragma once
#include <variant>

namespace state
{
    struct A {};
    struct B {};
    struct C {};

    using any = std::variant<A,B,C>;
}
