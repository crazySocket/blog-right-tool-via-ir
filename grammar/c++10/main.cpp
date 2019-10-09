#include <boost/format.hpp>
#include <iostream>
#include <variant>
#include <vector>

#include "frame.hpp"
#include "parser.hpp"
#include "rules.hpp"
#include "symbol.hpp"
#include "to_string.hpp"

using std::cout;
using std::endl;

int main()
{
    const std::string_view input = "text:hello world\nasdf:qwer\n";
    auto input_it = input.begin();

    data::message message;

    std::vector<symbol::any> queue;
    std::vector<frame::any> backtrace;
    queue.push_back(symbol::message{});
    backtrace.push_back(frame::message{&message});

    while(not queue.empty())
    {
        cout << to_string(queue) << endl;

        symbol::any symbol = queue.back();
        queue.pop_back();
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
            queue.push_back(symbol);
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
