#include <iostream>

#include "fsm.hpp"
#include "message.hpp"
#include "machine.hpp"
#include "rules.hpp"
#include "state.hpp"
#include "to_string.hpp"

using std::cout;
using std::endl;

int main()
{
    my_machine_t machine;
    cout << to_string(machine) << endl;
    fsm::storage::store(machine, state::B{});
    cout << to_string(machine) << endl;
    fsm::dispatch(machine, message::beta{});
    cout << to_string(machine) << endl;
}
