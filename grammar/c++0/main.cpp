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

void print_queue(std::vector<enum symbol> & queue)
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
    std::vector<enum symbol> queue;
    queue.push_back(symbol::start);

    std::string key;
    std::string value;
    std::string * field;

    while(not queue.empty() and input_it != input.end())
    {
        print_queue(queue);

        const enum symbol symbol = queue.back();
        queue.pop_back();

        const char next_char = *input_it;

        cout << boost::format("next symbol '%s'\n") % to_string(symbol);
        cout << boost::format("next char: '%x'\n") % int(next_char);

        switch (symbol) {
            case symbol::start:
                queue.push_back(symbol::message);
            break;
            case symbol::message:
                queue.push_back(symbol::character_nl);
                queue.push_back(symbol::line);
            break;
            case symbol::line:
                queue.push_back(symbol::value);
                queue.push_back(symbol::character_colon);
                queue.push_back(symbol::key);
            break;
            case symbol::key:
                field = &key;
                queue.push_back(symbol::text);
            break;
            case symbol::value:
                field = &value;
                queue.push_back(symbol::text);
            break;
            case symbol::text:
                queue.push_back(symbol::character_more);
                queue.push_back(symbol::character);
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
                    queue.push_back(symbol::character_more);
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
