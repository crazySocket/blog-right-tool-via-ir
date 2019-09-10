#include <type_traits>

template<typename T>
constexpr bool is_true = std::is_base_of_v<std::true_type, T>;

template<typename... Ts>
struct type_sequence {};

template<typename S, template<typename> typename P>
struct is_any
{
    static constexpr bool value = false;
};

template<template<typename> typename P, typename H, typename... R>
struct is_any<type_sequence<H, R...>, P> : is_any<type_sequence<R...>, P>
{
    static constexpr bool recursion = is_any<type_sequence<R...>, P>::value;
    static constexpr bool value = P<H>::value or recursion;
};
