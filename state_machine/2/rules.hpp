#pragma once
#include "rules.hpp"

#include "fsm.hpp"
#include "machine.hpp"
#include "message.hpp"

template<>
fsm::transition::any fsm::transit(my_machine_t & m, state::B &, message::beta);
