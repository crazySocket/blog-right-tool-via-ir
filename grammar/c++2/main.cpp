#include <boost/format.hpp>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;

namespace data
{
    struct text
    {
        std::string content;
    };

    struct key : text {};
    struct value : text {};

    struct line
    {
        struct key key;
        struct value value;
    };

    struct message
    {
        std::vector<line> lines;
    };
}

bool parse(std::string_view::iterator & input_it, std::string_view::iterator input_end, data::text & text)
{
    cout << __PRETTY_FUNCTION__ << endl;

    for(; input_it != input_end; input_it++)
    {
        char next_char = *input_it;
        if(isalnum(next_char) || next_char == ' ')
        {
            text.content.push_back(next_char);
        }
        else
        {
            break;
        }
    }
    return not text.content.empty();
}

bool parse(std::string_view::iterator & input_it, std::string_view::iterator input_end, data::value & value)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return parse(input_it, input_end, dynamic_cast<data::text&>(value));
}

bool parse(std::string_view::iterator & input_it, std::string_view::iterator input_end, data::key & key)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return parse(input_it, input_end, dynamic_cast<data::text&>(key));
}

bool parse(std::string_view::iterator & input_it, std::string_view::iterator input_end, data::line & line)
{
    cout << __PRETTY_FUNCTION__ << endl;

    bool successful;
    successful = parse(input_it, input_end, line.key);
    if(not successful)
    {
        return false;
    }

    char next_char = *input_it;
    if(next_char != ':')
    {
        return false;
    }
    else
    {
        input_it++;
    }

    successful = parse(input_it, input_end, line.value);
    if(not successful)
    {
        return false;
    }

    return true;
}

bool parse(std::string_view::iterator & input_it, std::string_view::iterator input_end, data::message & message)
{
    cout << __PRETTY_FUNCTION__ << endl;

    while(input_it != input_end)
    {
        message.lines.push_back({});
        bool successful = parse(input_it, input_end, message.lines.back());
        if(not successful)
        {
            return false;
        }
        input_it++;
    }
    return true;
}

int main()
{
    const std::string_view input = "text:hello world\n";

    data::message message;

    auto input_it = input.begin();
    bool successful = parse(input_it, input.end(), message);

    if(successful)
    {
        cout << "string matches\n";
        for(auto line : message.lines)
        {
            cout << boost::format("key: '%s', value: '%s'\n") % line.key.content % line.value.content;
        }
    }
    else
    {
        cout << "string does not match\n";
    }

    return 0;
}
