#include "to_string.hpp"

#include <boost/format.hpp>
#include <cctype>
#include <sstream>

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

std::string to_string(std::vector<symbol::any> & queue)
{
    std::stringstream ss;
    ss << "queue\n{\n";
    for(auto it = queue.rbegin(); it != queue.rend(); it++)
    {
        ss << (boost::format("\t%s\n") % to_string(*it)).str();
    }
    ss << "}";
    return ss.str();
}
