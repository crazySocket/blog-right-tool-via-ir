#pragma once
#include <tuple>
#include <variant>

#include "utils/wrap.hpp"

namespace fsm
{
    namespace storage
    {
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
}

namespace fsm
{
    namespace storage
    {
        namespace with_memory
        {
            template<typename ...Ts>
            struct machine;

            namespace impl
            {
                template<typename T>
                struct marker
                {
                    using type = T;
                };

                template<typename S, typename ...T>
                void mark(machine<T...>& m)
                {
                    m.m = marker<S>();
                }
            }

            template<typename ...Ts>
            struct machine
            {
                wrap_t<impl::marker, std::variant<Ts...>> m;
                std::tuple<Ts...> t;
            };

            template<typename S, typename ...T>
            S& get(machine<T...>& m)
            {
                return std::get<S>(m.t);
            }

            template<typename F, typename ...T>
            auto visit(F&& f, machine<T...>& m)
            {
                return std::visit(
                    [&](auto marker)
                    {
                        return f(get<typename decltype(marker)::type>(m));
                    },
                    m.m
                );
            }
        }

        template<typename ... Ts, typename S>
        void store(with_memory::machine<Ts...> & m, S s)
        {
            with_memory::impl::mark<S>(m);
            with_memory::get<S>(m) = std::forward<S>(s);
        }
    }
}
