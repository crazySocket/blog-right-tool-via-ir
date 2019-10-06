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

void print_queue(const std::vector<symbols::all> & queue)
{
    cout << "queue" << endl << '{' << endl;
    for(const auto & i : reverse(queue))
    {
        cout << '\t' << to_string(i) << endl;
    }
    cout << '}' << endl;
}

int main()
{
    char input[] = "text:hello world\n";
    auto input_it = std::begin(input);
    std::vector<symbols::all> queue;

    queue.push_back(symbols::S());

    while(not queue.empty() and input_it != std::end(input))
    {
        auto symbol = queue.back();
        char next_char = *input_it;

        print_queue(queue);
        cout << next_char << endl;

        bool pattern_matches_so_far = std::visit(
            [next_char, &queue, &input_it](auto symbol)
            {
                return rules::apply_rule(queue, input_it, symbol, next_char);
            },
            symbol
        );

        if(not pattern_matches_so_far)
        {
            break;
        }
    }

    if(queue.empty())
    {
        cout << "text matches grammar" << endl;
    }
    else
    {
        cout << "text does not match grammar" << endl;
    }
}
