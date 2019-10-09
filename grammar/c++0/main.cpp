#include <boost/format.hpp>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;

enum class symbol
{
    start,
    message,
    line,
    key,
    value,
    text,
    character,
    character_more,
    character_colon,
    character_nl
};

const char * to_string(enum symbol symbol)
{
#define SYMBOL_CASE(x) case symbol::x: return "symbol::" #x;
    switch (symbol)
    {
        SYMBOL_CASE(start)
        SYMBOL_CASE(message)
        SYMBOL_CASE(line)
        SYMBOL_CASE(key)
        SYMBOL_CASE(value)
        SYMBOL_CASE(text)
        SYMBOL_CASE(character)
        SYMBOL_CASE(character_more)
        SYMBOL_CASE(character_colon)
        SYMBOL_CASE(character_nl)
        default:
            return "unknown";
    }
}

void print_stack(std::vector<enum symbol> & stack)
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
    std::vector<enum symbol> stack;
    stack.push_back(symbol::start);

    std::string key;
    std::string value;
    std::string * field;

    while(not stack.empty() and input_it != input.end())
    {
        print_stack(stack);

        const enum symbol symbol = stack.back();
        stack.pop_back();

        const char next_char = *input_it;

        cout << boost::format("next symbol '%s'\n") % to_string(symbol);
        cout << boost::format("next char: '%x'\n") % int(next_char);

        switch (symbol) {
            case symbol::start:
                stack.push_back(symbol::message);
            break;
            case symbol::message:
                stack.push_back(symbol::character_nl);
                stack.push_back(symbol::line);
            break;
            case symbol::line:
                stack.push_back(symbol::value);
                stack.push_back(symbol::character_colon);
                stack.push_back(symbol::key);
            break;
            case symbol::key:
                field = &key;
                stack.push_back(symbol::text);
            break;
            case symbol::value:
                field = &value;
                stack.push_back(symbol::text);
            break;
            case symbol::text:
                stack.push_back(symbol::character_more);
                stack.push_back(symbol::character);
            break;
            case symbol::character:
                if(isalnum(next_char) || next_char == ' ')
                {
                    input_it++;
                    field->push_back(next_char);
                }
            break;
            case symbol::character_more:
                if(isalnum(next_char) || next_char == ' ')
                {
                    input_it++;
                    stack.push_back(symbol::character_more);
                    field->push_back(next_char);
                }
            break;
            case symbol::character_colon:
                if(next_char == ':')
                {
                    input_it++;
                }
                else
                {
                    goto exit;
                }
            break;
            case symbol::character_nl:
                if(next_char == '\n')
                {
                    input_it++;
                }
                else
                {
                    goto exit;
                }
            break;
        }
        getchar();
    }

    exit:
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
