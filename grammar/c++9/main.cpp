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
    struct character
    {
        char ch;
    };

    using any = std::variant<message, line, key, value, character>;
}

#define TO_STRING_FUNCTION(x) std::string to_string(x) { return #x; }
TO_STRING_FUNCTION(symbol::message)
TO_STRING_FUNCTION(symbol::line)
TO_STRING_FUNCTION(symbol::key)
TO_STRING_FUNCTION(symbol::value)

std::string to_string(char ch)
{
    if(std::isgraph(ch))
    {
        return (boost::format("`%c'") % ch).str();
    }
    else
    {
        return (boost::format("0x%x") % static_cast<int>(ch)).str();
    }
}

std::string to_string(symbol::character symbol)
{
    return (boost::format("symbol::character: %s") % to_string(symbol.ch)).str();
}

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
        struct mismatch {};
        struct consumed {};
        struct finished
        {
            bool consume;
            bool frame;
        };
        struct append_symbols
        {
            std::list<symbol::any> symbols;
        };
        struct change_frame
        {
            frame::any frame;
        };
        struct any : std::variant<mismatch, consumed, finished, append_symbols, change_frame> {};
    }

    template<typename S, typename F>
    result::any parse(S symbol, F & frame, char next_char)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        return {result::mismatch{}};
    }

    template<typename S, typename F>
    result::any parse(S symbol, F & frame)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        return {result::mismatch{}};
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
            return {result::mismatch{}};
        }
        else
        {
            return {result::finished{.consume = false, .frame = true}};
        }
    }
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
parser::result::any parser::parse(symbol::character symbol, frame::line & frame, char next_char)
{
    cout << __PRETTY_FUNCTION__ << endl;
    switch (symbol.ch)
    {
        case ':':
            return {result::finished{.consume = true, .frame = false}};
        case '\n':
            return {result::finished{.consume = true, .frame = true}};
        default:
            return {result::mismatch{}};
    }
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
            .symbols = {symbol::key{}, symbol::character{':'}, symbol::value{}, symbol::character{'\n'}}
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
            .symbols = {symbol::line{}, symbol::message{}}
        }
    };
}

int main()
{
    const std::string_view input = "text:hello world\nasdf:qwer\n";
    auto input_it = input.begin();

    data::message message;

    std::vector<symbol::any> stack;
    std::vector<frame::any> backtrace;
    stack.push_back(symbol::message{});
    backtrace.push_back(frame::message{&message});

    while(not stack.empty())
    {
        print_stack(stack);

        symbol::any symbol = stack.back();
        stack.pop_back();
        frame::any & frame = backtrace.back();

        cout << boost::format("next symbol: `%s'\n") % to_string(symbol);

        parser::result::any result;

        if(input_it != input.end())
        {
            const char next_char = *input_it;

            cout << boost::format("next char: %s\n") % to_string(next_char);

            result = std::visit(
                [next_char](auto symbol, auto & frame)
                {
                    return parser::parse(symbol, frame, next_char);
                },
                symbol, frame
            );
        }
        else
        {
            result = std::visit(
                [](auto symbol, auto & frame)
                {
                    return parser::parse(symbol, frame);
                },
                symbol, frame
            );
        }

        if(std::holds_alternative<parser::result::mismatch>(result))
        {
            break;
        }
        else if(std::holds_alternative<parser::result::consumed>(result))
        {
            input_it++;
            stack.push_back(symbol);
        }
        else if(std::holds_alternative<parser::result::finished>(result))
        {
            auto result_finished = std::get<parser::result::finished>(result);
            if(result_finished.consume)
            {
                input_it++;
            }
            if(result_finished.frame)
            {
                backtrace.pop_back();
            }
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
                stack.push_back(symbol);
            }
        }
        else if(std::holds_alternative<parser::result::change_frame>(result))
        {
            backtrace.push_back(std::get<parser::result::change_frame>(result).frame);
            stack.push_back(symbol);
        }

        getchar();
    }

    if(stack.empty() and input_it == input.end())
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
