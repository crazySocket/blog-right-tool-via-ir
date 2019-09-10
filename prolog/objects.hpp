#include "utils.hpp"

struct A {};
struct B {};
struct C {};
struct D {};

using all_objects = type_sequence<A,B,C,D>;
