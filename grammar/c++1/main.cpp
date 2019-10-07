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

void print_queue(std::vector<symbol::any> & queue)
{
    cout << "queue\n{\n";
    for(auto it = queue.rbegin(); it != queue.rend(); it++)
    {
        cout << boost::format("\t%s\n") % to_string(*it);
    }
    cout << "}\n";
}

int main()
{
    const std::string_view input = "text:hello world\n";
    auto input_it = input.begin();
    std::vector<symbol::any> queue;
    queue.push_back(symbol::start{});

    std::string key;
    std::string value;
    std::string * field;

    while(not queue.empty() and input_it != input.end())
    {
        print_queue(queue);

        const symbol::any symbol = queue.back();
        queue.pop_back();

        const char next_char = *input_it;

        cout << boost::format("next symbol '%s'\n") % to_string(symbol);
        cout << boost::format("next char: '%x'\n") % int(next_char);

        bool matches_so_far = true;

        std::visit(
            overloaded
            {
                [&](symbol::start)
                {
                    queue.push_back(symbol::message{});
                },
                [&](symbol::message)
                {
                    queue.push_back(symbol::character{'\n'});
                    queue.push_back(symbol::line{});
                },
                [&](symbol::line)
                {
                    queue.push_back(symbol::value{});
                    queue.push_back(symbol::character{':'});
                    queue.push_back(symbol::key{});
                },
                [&](symbol::key)
                {
                    field = &key;
                    queue.push_back(symbol::text{});
                },
                [&](symbol::value)
                {
                    field = &value;
                    queue.push_back(symbol::text{});
                },
                [&](symbol::text)
                {
                    queue.push_back(symbol::character_more{});
                    queue.push_back(symbol::character_any{});
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
                        queue.push_back(symbol::character_more{});
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

    if(queue.empty() && input_it == input.end())
    {
        cout << "string matches\n";
        cout << boost::format("key: '%s', value: '%s'\n") % key % value;
    }
    else
    {
        cout << "string does not match\n";
        print_queue(queue);
    }

    return 0;
}
