#include "common.hpp"
using namespace caf;



struct cleint_state {

};

behavior client_actor(stateful_actor<cleint_state> *self) {
  aout(self) << " ######## Starting Client Actor ######## \n";
  
  uint16_t port = 4444;
  auto server = self->system().middleman().remote_actor("172.17.0.3", 4444);
  

  return {
    [=](new_connection) {

    }
  };
}

void caf_main(actor_system& system) {
  scoped_actor self{system};
  aout(self) << " ######## Starting Client System ######## \n";
}

CAF_MAIN(io::middleman)
