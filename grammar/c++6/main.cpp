#include <boost/format.hpp>
#include <functional>
#include <iostream>
#include <variant>
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

struct parse_fn_res;

using parse_fn = std::function<parse_fn_res(char)>;

struct error {};
struct consumed {};
struct rejected {};
struct next_parser
{
    parse_fn parser;
};

struct parse_fn_res : std::variant<error, consumed, rejected, next_parser> {};

struct text_parser
{
    data::text * current_text;

    parse_fn_res operator()(char next_char)
    {
        cout << __PRETTY_FUNCTION__ << endl;

        if(isalnum(next_char) || next_char == ' ')
        {
            current_text->content.push_back(next_char);
            return {consumed{}};
        }
        else
        {
            if(current_text->content.empty())
            {
                return {error{}};
            }
            else
            {
                return {rejected{}};
            }
        }
    }
};

struct line_parser
{
    data::line * current_line;
    enum class stage
    {
        next_line,
        parsed_key,
        parsed_value
    } current_stage;

    line_parser(data::line * current_line)
    : current_line(current_line), current_stage(stage::next_line)
    {}

    parse_fn_res operator()(char ch)
    {
        cout << __PRETTY_FUNCTION__ << endl;

        if(current_stage == stage::next_line)
        {
            current_stage = stage::parsed_key;
            return {next_parser{text_parser{&current_line->key}}};
        }
        else if(ch == ':')
        {
            return {consumed{}};
        }
        else if(current_stage == stage::parsed_key)
        {
            current_stage = stage::parsed_value;
            return {next_parser{text_parser{&current_line->value}}};
        }
        else if(ch == '\n')
        {
            return {consumed{}};
        }
        else
        {
            return {rejected{}};
        }
    }
};

struct message_parser
{
    data::message * message;

    parse_fn_res operator()(char)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        message->lines.push_back({});
        return {next_parser{line_parser{&message->lines.back()}}};
    }
};

int main()
{
    const std::string_view input = "text:hello world\n";
    auto input_it = input.begin();

    std::vector<parse_fn> backtrace;
    data::message message;
    backtrace.push_back(message_parser{&message});

    while(not backtrace.empty() and input_it != input.end())
    {
        parse_fn & parser = backtrace.back();
        const char next_char = *input_it;

        cout << boost::format("next char: '%x'\n") % int(next_char);

        auto result = parser(next_char);
        if(std::holds_alternative<error>(result))
        {
            break;
        }
        else if(std::holds_alternative<consumed>(result))
        {
            input_it++;
        }
        else if(std::holds_alternative<next_parser>(result))
        {
            backtrace.push_back(std::get<next_parser>(result).parser);
        }
        else if(std::holds_alternative<rejected>(result))
        {
            backtrace.pop_back();
        }
        getchar();
    }

    if(backtrace.size() > 1 && input_it == input.end())
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
