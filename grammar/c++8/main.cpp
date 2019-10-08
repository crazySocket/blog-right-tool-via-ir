#include <boost/format.hpp>
#include <iostream>
#include <list>
#include <variant>
#include <vector>
using std::cout;
using std::endl;

namespace symbol
{
    struct message {};
    struct line {};
    struct key {};
    struct value {};
    struct colon {};
    struct new_line {};

    using any = std::variant<message, line, key, value, colon, new_line>;
}

#define TO_STRING_FUNCTION(x) std::string to_string(x) { return #x; }
TO_STRING_FUNCTION(symbol::message)
TO_STRING_FUNCTION(symbol::line)
TO_STRING_FUNCTION(symbol::key)
TO_STRING_FUNCTION(symbol::value)
TO_STRING_FUNCTION(symbol::colon)
TO_STRING_FUNCTION(symbol::new_line)

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
        struct consumed
        {
            bool do_continue = true;
        };
        struct rejected {};
        struct append_symbols
        {
            std::list<symbol::any> symbols;
        };
        struct change_frame
        {
            frame::any frame;
        };
        struct any : std::variant<error, consumed, rejected, append_symbols, change_frame> {};
    }

    template<typename S, typename F>
    result::any parse(S symbol, F & frame, char next_char)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        exit(1);
    }
}

template<typename S>
parser::result::any parser::parse(S, frame::text & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    if(isalnum(next_char) || next_char == ' ')
    {
        frame.current_text->content.push_back(next_char);
        return {result::consumed{}};
    }
    else
    {
        if(frame.current_text->content.empty())
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
parser::result::any parser::parse(symbol::new_line, frame::line & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return {result::consumed{.do_continue = false}};
}

template<>
parser::result::any parser::parse(symbol::value, frame::line & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return
    {
        result::change_frame
        {
            .frame = frame::text{&frame.current_line->value}
        }
    };
}

template<>
parser::result::any parser::parse(symbol::colon, frame::line & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return {result::consumed{.do_continue = false}};
}

template<>
parser::result::any parser::parse(symbol::key, frame::line & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return
    {
        result::change_frame
        {
            .frame = frame::text{&frame.current_line->key}
        }
    };
}

template<>
parser::result::any parser::parse(symbol::line, frame::line & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return
    {
        result::append_symbols
        {
            .symbols = {symbol::key{}, symbol::colon{}, symbol::value{}, symbol::new_line{}}
        }
    };
}

template<>
parser::result::any parser::parse(symbol::line, frame::message & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    frame.message->lines.push_back({});
    return
    {
        result::change_frame
        {
            .frame = frame::line{&frame.message->lines.back()}
        }
    };
}

template<>
parser::result::any parser::parse(symbol::message, frame::message & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    return
    {
        result::append_symbols
        {
            .symbols = {symbol::line{}}
        }
    };
}

int main()
{
    const std::string_view input = "text:hello world\n";
    auto input_it = input.begin();

    data::message message;

    std::vector<symbol::any> queue;
    std::vector<frame::any> backtrace;
    queue.push_back(symbol::message{});
    backtrace.push_back(frame::message{&message});

    while(not queue.empty() and input_it != input.end())
    {
        print_queue(queue);

        symbol::any symbol = queue.back();
        queue.pop_back();
        frame::any & frame = backtrace.back();
        const char next_char = *input_it;

        cout << boost::format("next symbol '%s'\n") % to_string(symbol);
        cout << boost::format("next char: '%x'\n") % int(next_char);

        auto result = std::visit(
            [next_char](auto symbol, auto & frame)
            {
                return parser::parse(symbol, frame, next_char);
            },
            symbol, frame
        );

        if(std::holds_alternative<parser::result::error>(result))
        {
            break;
        }
        else if(std::holds_alternative<parser::result::consumed>(result))
        {
            input_it++;
            if(std::get<parser::result::consumed>(result).do_continue)
            {
                queue.push_back(symbol);
            }
        }
        else if(std::holds_alternative<parser::result::rejected>(result))
        {
            backtrace.pop_back();
        }
        else if(std::holds_alternative<parser::result::append_symbols>(result))
        {
            auto symbols = std::get<parser::result::append_symbols>(result).symbols;
            if(symbols.empty())
            {
                std::cerr << "symbol list must not be empty!\n";
                exit(1);
            }
            symbols.reverse();
            for(auto symbol : symbols)
            {
                queue.push_back(symbol);
            }
        }
        else if(std::holds_alternative<parser::result::change_frame>(result))
        {
            backtrace.push_back(std::get<parser::result::change_frame>(result).frame);
            queue.push_back(symbol);
        }

        getchar();
    }

    if(queue.empty() && input_it == input.end())
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