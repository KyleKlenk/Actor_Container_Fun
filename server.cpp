#include "common.hpp"
using namespace caf;





struct server_state {

};

behavior server_actor(stateful_actor<server_state> *self) {
  aout(self) << " ######## Starting Server Actor ######## \n";

  auto config_opt = parse_config("./settings.toml");
  auto config = *config_opt;

  std::string server = config["server"].value_or("localhost");
  aout(self) << "Server: " << server << "\n";
  uint16_t port = config["port"].value_or(4444);
  aout(self) << "Port: " << port << "\n";

  auto is_port = self->system().middleman().publish(self, port);
  if(!is_port) {
    aout(self) << "Failed To Publish Actor On Port: " << port << "\n"; return {};}


  


  return {
    [=](new_connection) {
      aout(self) << "Recieved Connection Request\n";
      auto sender_addr = actor_cast<actor>(self->current_sender());

      self->spawn(listener, sender_addr);

      self->send(sender_addr, "Identify Yourself !!");
    },

    [=](std::string& name) {
      aout(self) << name << " is trying to connect....\n";
      
    }
  };
}

void caf_main(actor_system& system) {
  scoped_actor self{system};
  aout(self) << " ######## Starting Actor System ######## \n";
  self->spawn(server_actor);
}

CAF_MAIN(io::middleman, id_block::app)
