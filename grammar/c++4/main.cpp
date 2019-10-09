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

data::message message;
data::line * current_line;
data::key * current_key;
data::value * current_value;
data::text * current_text;

struct parse_fn_res;

using parse_fn = parse_fn_res(*)(char);

struct parse_fn_res
{
    bool error;
    bool consumed;
    parse_fn next_parser;
};

parse_fn_res parse_text(char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;

    if(isalnum(next_char) || next_char == ' ')
    {
        current_text->content.push_back(next_char);
        return
        {
            .error = false,
            .consumed = true,
            .next_parser = nullptr
        };
    }
    else
    {
        return
        {
            .error = current_text->content.empty(),
            .consumed = false,
            .next_parser = nullptr
        };
    }
}

parse_fn_res parse_value(char ch)
{
    cout << __PRETTY_FUNCTION__ << endl;
    current_text = current_value;
    return parse_text(ch);
}

parse_fn_res parse_key(char ch)
{
    cout << __PRETTY_FUNCTION__ << endl;
    current_text = current_key;
    return parse_text(ch);
}

parse_fn_res parse_line(char ch)
{
    cout << __PRETTY_FUNCTION__ << endl;

    if(current_key == nullptr)
    {
        current_key = &current_line->key;
        return
        {
            .error = false,
            .consumed = false,
            .next_parser = parse_key
        };
    }
    else if(ch == ':')
    {
        return
        {
            .error = false,
            .consumed = true,
            .next_parser = nullptr
        };
    }
    else if(current_value == nullptr)
    {
        current_value = &current_line->value;
        return
        {
            .error = false,
            .consumed = false,
            .next_parser = parse_value
        };
    }
    else if(ch == '\n')
    {
        current_key = nullptr;
        current_value = nullptr;
        return
        {
            .error = false,
            .consumed = true,
            .next_parser = nullptr
        };
    }
    else
    {
        return
        {
            .error = true,
            .consumed = false,
            .next_parser = nullptr
        };
    }
}

parse_fn_res parse_message(char)
{
    cout << __PRETTY_FUNCTION__ << endl;

    message.lines.push_back({});
    current_line = &message.lines.back();

    return
    {
        .error = false,
        .consumed = false,
        .next_parser = parse_line
    };
}

int main()
{
    const std::string_view input = "text:hello world\n";
    auto input_it = input.begin();

    std::vector<parse_fn> backtrace;
    backtrace.push_back(parse_message);

    while(not backtrace.empty() and input_it != input.end())
    {
        parse_fn parser = backtrace.back();
        const char next_char = *input_it;

        cout << boost::format("next char: '%x'\n") % int(next_char);

        auto result = parser(next_char);
        if(result.error)
        {
            break;
        }
        else if(result.consumed)
        {
            input_it++;
        }
        else if(result.next_parser)
        {
            backtrace.push_back(result.next_parser);
        }
        else // rejected
        {
            backtrace.pop_back();
        }
        getchar();
    }

    if(backtrace.size() > 1 and input_it == input.end())
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
