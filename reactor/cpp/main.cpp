#include <stdio.h>
#include "rxcpp/rx.hpp"

int main()
{
    auto values = rxcpp::observable<>::range(1, 3)
        .map([](int v) {
            return 2 * v;
        });
    values
        .subscribe(
            [](int v){printf("OnNext: %d\n", v);},
            [](){printf("OnCompleted\n");}
        );
}
