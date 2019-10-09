#pragma once
#include <variant>

#include "state.hpp"
#include "storage.hpp"

namespace fsm
{
    namespace storage
    {
        template<typename M, typename S>
        void store(M & m, S s);
    }

    namespace transition
    {
        struct no_action {};
        struct change
        {
            state::any next_state;
        };

        using any = std::variant<no_action, change>;
    }

    template<typename M, typename S, typename E>
    transition::any transit(M & m, S & s, E e)
    {
        return transition::no_action{};
    }

    template<typename M, typename E>
    void dispatch(M & m, E e)
    {
        auto result = visit(
            [&](auto s)
            {
                return transit(m, s, e);
            },
            m
        );

        if(std::holds_alternative<transition::no_action>(result))
        {
            return;
        }
        else if(std::holds_alternative<transition::change>(result))
        {
            visit(
                [&](auto s)
                {
                    storage::store(m, s);
                },
                std::get<transition::change>(result).next_state
            );
        }
    }
}
