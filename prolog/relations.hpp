#include <type_traits>

template<typename T, typename R>
struct father : std::false_type {};

template<>
struct father<A,B> : std::true_type {};

template<>
struct father<B,D> : std::true_type {};
