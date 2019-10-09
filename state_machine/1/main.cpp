#include <iostream>
#include <tuple>
#include <variant>
using std::cout;
using std::endl;

namespace state
{
    struct A {};
    struct B {};
    struct C {};

    using any = std::variant<A,B,C>;
}

#define TO_STRING_IMPL(x) std::string to_string(x) { return #x; }
TO_STRING_IMPL(state::A)
TO_STRING_IMPL(state::B)
TO_STRING_IMPL(state::C)
#undef TO_STRING_IMPL

namespace message
{
    struct alpha {};
    struct beta {};
    struct gamma {};
}

namespace fsm
{
    namespace storage
    {
        template<typename M, typename S>
        void store(M & m, S s);

        namespace without_memory
        {
            template<typename ... Ts>
            using machine = std::variant<Ts...>;
        }

        template<typename ... Ts, typename S>
        void store(without_memory::machine<Ts...> & m, S s)
        {
            m = s;
        }
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
        auto result = std::visit(
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
            storage::store(m, std::get<transition::change>(result).next_state);
        }
    }
}

template<typename ... Ts>
std::string to_string(fsm::storage::without_memory::machine<Ts...> & m)
{
    return std::visit(
        [](auto s)
        {
            return to_string(s);
        },
        m
    );
}

using my_machine_t = fsm::storage::without_memory::machine<state::A, state::B, state::C>;

template<>
fsm::transition::any fsm::transit(my_machine_t & m, state::B &, message::beta)
{
    return fsm::transition::change{state::C{}};
}

int main()
{
    my_machine_t machine;
    cout << to_string(machine) << endl;
    fsm::storage::store(machine, state::B{});
    cout << to_string(machine) << endl;
    fsm::dispatch(machine, message::beta{});
    cout << to_string(machine) << endl;
}
