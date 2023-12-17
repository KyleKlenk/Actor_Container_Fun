#include "common.hpp"
using namespace caf;



struct cleint_state {

};

behavior client_actor(stateful_actor<cleint_state> *self) {
  aout(self) << " ######## Starting Client Actor ######## \n";

  auto config_opt = parse_config("./settings.toml");
  auto config = *config_opt;

  std::string server = config["server"].value_or("localhost");
  aout(self) << "Server: " << server << "\n";
  uint16_t port = config["port"].value_or(4444);
  aout(self) << "Port: " << port << "\n";

  auto server_actor = self->system().middleman().remote_actor(server, port);
  self->monitor(*server_actor);
  aout(self) << "Connected To Server\n";
  self->send(*server_actor, new_connection_v);
  

  return {
    [=](new_connection) {

    }
  };
}

void caf_main(actor_system& system) {
  scoped_actor self{system};
  aout(self) << " ######## Starting Client System ######## \n";
  self->spawn(client_actor);
}

CAF_MAIN(io::middleman, id_block::app)
