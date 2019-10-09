#include "rules.hpp"

template<>
fsm::transition::any fsm::transit(my_machine_t & m, state::B &, message::beta)
{
    return fsm::transition::change{state::C{}};
}
