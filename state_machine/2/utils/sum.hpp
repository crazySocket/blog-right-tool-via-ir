#pragma once
#include <variant>

namespace impl
{
    template<typename ...T>
    struct sum;

    template<typename T>
    struct sum<T>
    {
        using type = std::variant<T>;
    };

    template<typename ...T>
    struct sum<std::variant<T...>>
    {
        using type = std::variant<T...>;
    };

    template<typename ...T, typename ...R, typename ...S>
    struct sum<std::variant<T...>, std::variant<R...>, S...>
    {
        using type = typename sum<std::variant<T..., R...>, S...>::type;
    };

    template<typename ...T, typename ...S>
    struct sum<std::variant<T...>, void, S...>
    {
        using type = typename sum<std::variant<T...>, S...>::type;
    };
}

template<typename ...T>
using sum_t = typename impl::sum<T...>::type;
