#include <iostream>
#include <vector>
#include "symbols.hpp"
#include "rules.hpp"

using std::cout;
using std::endl;

template<typename T>
struct reverse_wrapper
{
    T & iterable;
};

template<typename T>
reverse_wrapper<T> reverse(T & t)
{
    return {t};
}

template<typename T>
auto begin(reverse_wrapper<T> & wrapper)
{
    return std::rbegin(wrapper.iterable);
}

template<typename T>
auto end(reverse_wrapper<T> & wrapper)
{
    return std::rend(wrapper.iterable);
}

void print_stack(const std::vector<symbols::all> & stack)
{
    cout << "stack" << endl << '{' << endl;
    for(const auto & i : reverse(stack))
    {
        cout << '\t' << to_string(i) << endl;
    }
    cout << '}' << endl;
}

int main()
{
    char input[] = "text:hello world\n";
    auto input_it = std::begin(input);
    std::vector<symbols::all> stack;

    stack.push_back(symbols::S());

    while(not stack.empty() and input_it != std::end(input))
    {
        auto symbol = stack.back();
        char next_char = *input_it;

        print_stack(stack);
        cout << next_char << endl;

        bool pattern_matches_so_far = std::visit(
            [next_char, &stack, &input_it](auto symbol)
            {
                return rules::apply_rule(stack, input_it, symbol, next_char);
            },
            symbol
        );

        if(not pattern_matches_so_far)
        {
            break;
        }
    }

    if(stack.empty())
    {
        cout << "text matches grammar" << endl;
    }
    else
    {
        cout << "text does not match grammar" << endl;
    }
}
