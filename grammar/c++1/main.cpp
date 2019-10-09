#include <boost/format.hpp>
#include <iostream>
#include <variant>
#include <vector>

#include "overloaded.hpp"

using std::cout;
using std::endl;

namespace symbol
{
    struct start {};
    struct message {};
    struct line {};

    struct text {};

    struct key : text {};
    struct value : text {};
    struct character
    {
        char ch;
    };
    struct character_any {};
    struct character_more {};

    using any = std::variant<start, message, line, key, value, character, character_any, character_more, text>;
}

#define TO_STRING_FUNCTION(x) std::string to_string(x) { return #x; }
TO_STRING_FUNCTION(symbol::start)
TO_STRING_FUNCTION(symbol::message)
TO_STRING_FUNCTION(symbol::line)
TO_STRING_FUNCTION(symbol::key)
TO_STRING_FUNCTION(symbol::value)
TO_STRING_FUNCTION(symbol::text)

std::string to_string(symbol::character s)
{
    auto format = boost::format("symbol::character %x") % int(s.ch);
    return format.str();
}

TO_STRING_FUNCTION(symbol::character_any)
TO_STRING_FUNCTION(symbol::character_more)

std::string to_string(symbol::any variant)
{
    return std::visit([](auto x) { return to_string(x); }, variant);
}

void print_stack(std::vector<symbol::any> & stack)
{
    cout << "stack\n{\n";
    for(auto it = stack.rbegin(); it != stack.rend(); it++)
    {
        cout << boost::format("\t%s\n") % to_string(*it);
    }
    cout << "}\n";
}

int main()
{
    const std::string_view input = "text:hello world\n";
    auto input_it = input.begin();
    std::vector<symbol::any> stack;
    stack.push_back(symbol::start{});

    std::string key;
    std::string value;
    std::string * field;

    while(not stack.empty() and input_it != input.end())
    {
        print_stack(stack);

        const symbol::any symbol = stack.back();
        stack.pop_back();

        const char next_char = *input_it;

        cout << boost::format("next symbol '%s'\n") % to_string(symbol);
        cout << boost::format("next char: '%x'\n") % int(next_char);

        bool matches_so_far = true;

        std::visit(
            overloaded
            {
                [&](symbol::start)
                {
                    stack.push_back(symbol::message{});
                },
                [&](symbol::message)
                {
                    stack.push_back(symbol::character{'\n'});
                    stack.push_back(symbol::line{});
                },
                [&](symbol::line)
                {
                    stack.push_back(symbol::value{});
                    stack.push_back(symbol::character{':'});
                    stack.push_back(symbol::key{});
                },
                [&](symbol::key)
                {
                    field = &key;
                    stack.push_back(symbol::text{});
                },
                [&](symbol::value)
                {
                    field = &value;
                    stack.push_back(symbol::text{});
                },
                [&](symbol::text)
                {
                    stack.push_back(symbol::character_more{});
                    stack.push_back(symbol::character_any{});
                },
                [&](symbol::character_any s)
                {
                    if(isalnum(next_char) || next_char == ' ')
                    {
                        input_it++;
                        field->push_back(next_char);
                    }
                },
                [&](symbol::character_more s)
                {
                    if(isalnum(next_char) || next_char == ' ')
                    {
                        input_it++;
                        stack.push_back(symbol::character_more{});
                        field->push_back(next_char);
                    }
                },
                [&](symbol::character s)
                {
                    if(next_char == s.ch)
                    {
                        input_it++;
                    }
                    else
                    {
                        matches_so_far = false;
                    }
                }
            },
            symbol
        );

        if(not matches_so_far)
        {
            break;
        }

        getchar();
    }

    if(stack.empty() and input_it == input.end())
    {
        cout << "string matches\n";
        cout << boost::format("key: '%s', value: '%s'\n") % key % value;
    }
    else
    {
        cout << "string does not match\n";
        print_stack(stack);
    }

    return 0;
}
