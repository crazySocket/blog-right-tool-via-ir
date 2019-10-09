#pragma once

#include <iostream>
#include <vector>
#include "symbols.hpp"

using std::cout;
using std::endl;

namespace rules
{
    bool apply_rule(std::vector<symbols::all> & stack, char *& input_it, symbols::S &, char ch)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        stack.pop_back();
        stack.push_back(symbols::Message());
        return true;
    }

    bool apply_rule(std::vector<symbols::all> & stack, char *& input_it, symbols::Message &, char ch)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        stack.pop_back();
        stack.push_back(symbols::Line());
        return true;
    }

    bool apply_rule(std::vector<symbols::all> & stack, char *& input_it, symbols::Line &, char ch)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        stack.pop_back();
        stack.push_back(symbols::Value());
        stack.push_back(symbols::Character{':'});
        stack.push_back(symbols::Key());
        return true;
    }

    bool apply_rule(std::vector<symbols::all> & stack, char *& input_it, symbols::Text &, char ch)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        stack.pop_back();
        cout << ch << endl;
        if(std::isalnum(ch) or ch == ' ')
        {
            stack.push_back(symbols::Value());
            input_it++;
        }
        return true;
    }

    bool apply_rule(std::vector<symbols::all> & stack, char *& input_it, symbols::Key & symbol, char ch)
    {
        return apply_rule(stack, input_it, static_cast<symbols::Text &>(symbol), ch);
    }

    bool apply_rule(std::vector<symbols::all> & stack, char *& input_it, symbols::Value & symbol, char ch)
    {
        return apply_rule(stack, input_it, static_cast<symbols::Text &>(symbol), ch);
    }

    // bool apply_rule(std::vector<symbols::all> & stack, char *& input_it, symbols::Key &, char ch)
    // {
    //     cout << __PRETTY_FUNCTION__ << endl;
    //     stack.pop_back();
    //     cout << ch << endl;
    //     if(ch != ':')
    //     {
    //         stack.push_back(symbols::Key());
    //         input_it++;
    //     }
    //     return true;
    // }
    //
    // bool apply_rule(std::vector<symbols::all> & stack, char *& input_it, symbols::Value &, char ch)
    // {
    //     cout << __PRETTY_FUNCTION__ << endl;
    //     stack.pop_back();
    //     cout << ch << endl;
    //     if(ch != '\n')
    //     {
    //         stack.push_back(symbols::Value());
    //         input_it++;
    //     }
    //     return true;
    // }

    bool apply_rule(std::vector<symbols::all> & stack, char *& input_it, symbols::Character & symbol, char ch)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        stack.pop_back();
        if(symbol.ch == ch)
        {
            input_it++;
        }
        return true;
    }

    template<typename T>
    bool apply_rule(std::vector<symbols::all> & stack, char *& input_it, T &, char ch)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        return true;
    }
}
