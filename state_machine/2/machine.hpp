#pragma once
#include "state.hpp"
#include "fsm.hpp"

// using my_machine_t = fsm::storage::without_memory::machine<state::A, state::B, state::C>;
using my_machine_t = fsm::storage::with_memory::machine<state::A, state::B, state::C>;
