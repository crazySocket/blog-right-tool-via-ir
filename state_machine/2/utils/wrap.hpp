#pragma once
#include "sum.hpp"

namespace impl
{
    template<template<typename> typename W, typename T>
    struct wrap
    {
        using type = W<T>;
    };

    template<template<typename> typename W, typename T>
    struct wrap<W, std::variant<T>>
    {
        using type = std::variant<W<T>>;
    };

    template<template<typename> typename W, typename H, typename ...T>
    struct wrap<W, std::variant<H, T...>>
    {
        using type = sum_t<
            std::variant<typename wrap<W,H>::type>,
            typename wrap<W,std::variant<T...>>::type
        >;
    };
}

template<template<typename> typename W, typename T>
using wrap_t = typename impl::wrap<W, T>::type;
