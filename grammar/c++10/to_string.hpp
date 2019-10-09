#pragma once
#include <string>
#include <vector>

#include "symbol.hpp"

#define TO_STRING_FUNCTION(x) inline std::string to_string(x) { return #x; };
TO_STRING_FUNCTION(symbol::message)
TO_STRING_FUNCTION(symbol::line)
TO_STRING_FUNCTION(symbol::key)
TO_STRING_FUNCTION(symbol::value)
TO_STRING_FUNCTION(symbol::not_delimeter)
TO_STRING_FUNCTION(symbol::not_delimeter_more)
#undef TO_STRING_FUNCTION

std::string to_string(char ch);

std::string to_string(symbol::character symbol);

std::string to_string(symbol::any variant);

std::string to_string(std::vector<symbol::any> & stack);
