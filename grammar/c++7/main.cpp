#include <boost/format.hpp>
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

    struct line
    {
        struct text key;
        struct text value;
    };

    struct message
    {
        std::vector<line> lines;
    };
}

namespace frame
{
    struct text
    {
        data::text * current_text;
    };

    struct line
    {
        data::line * current_line;
        enum class stage
        {
            next_line,
            parsed_key,
            parsed_value
        } current_stage;

        line(data::line * current_line)
        : current_line(current_line), current_stage(stage::next_line)
        {}
    };

    struct message
    {
        data::message * message;
    };

    using any = std::variant<text, line, message>;
}

namespace parser
{
    namespace result
    {
        struct error {};
        struct consumed {};
        struct rejected {};
        struct next_parser
        {
            frame::any parser;
        };

        struct any : std::variant<error, consumed, rejected, next_parser> {};
    }

    template<typename T>
    result::any parse(T & self, char next_char);
}

template<>
parser::result::any parser::parse(frame::text & self, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;

    if(isalnum(next_char) || next_char == ' ')
    {
        self.current_text->content.push_back(next_char);
        return {result::consumed{}};
    }
    else
    {
        if(self.current_text->content.empty())
        {
            return {result::error{}};
        }
        else
        {
            return {result::rejected{}};
        }
    }
}

template<>
parser::result::any parser::parse(frame::line & self, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;

    if(self.current_stage == frame::line::stage::next_line)
    {
        self.current_stage = frame::line::stage::parsed_key;
        return {result::next_parser{frame::text{&self.current_line->key}}};
    }
    else if(next_char == ':')
    {
        return {result::consumed{}};
    }
    else if(self.current_stage == frame::line::stage::parsed_key)
    {
        self.current_stage = frame::line::stage::parsed_value;
        return {result::next_parser{frame::text{&self.current_line->value}}};
    }
    else if(next_char == '\n')
    {
        return {result::consumed{}};
    }
    else
    {
        return {result::rejected{}};
    }
}

template<>
parser::result::any parser::parse(frame::message & self, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    self.message->lines.push_back({});
    return {result::next_parser{frame::line{&self.message->lines.back()}}};
}

int main()
{
    const std::string_view input = "text:hello world\n";
    auto input_it = input.begin();

    std::vector<frame::any> backtrace;
    data::message message;
    backtrace.push_back(frame::message{&message});

    while(not backtrace.empty() and input_it != input.end())
    {
        frame::any & frame = backtrace.back();
        const char next_char = *input_it;

        cout << boost::format("next char: '%x'\n") % int(next_char);

        auto result = std::visit(
            [next_char](auto & frame)
            {
                return parser::parse(frame, next_char);
            },
            frame
        );

        if(std::holds_alternative<parser::result::error>(result))
        {
            break;
        }
        else if(std::holds_alternative<parser::result::consumed>(result))
        {
            input_it++;
        }
        else if(std::holds_alternative<parser::result::next_parser>(result))
        {
            backtrace.push_back(std::get<parser::result::next_parser>(result).parser);
        }
        else if(std::holds_alternative<parser::result::rejected>(result))
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
