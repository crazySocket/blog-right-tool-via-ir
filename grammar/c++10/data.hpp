#pragma once
#include <string>
#include <vector>

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
