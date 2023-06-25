#include <iostream>
#include <caf/all.hpp>
#include "caf/io/all.hpp"

using namespace caf;

/* Configuration class that handles the config and 
/  command line options for the actors program */
class config : public actor_system_config {
    public:
        int startGRU = -1;
        int countGRU = -1;
        std::string config_file = "";
        bool backup_server = false;
        bool server_mode = false;
    
    config() {
        opt_group{custom_options_, "global"}
            .add(startGRU, "gru,g", "Starting GRU Index")
            .add(countGRU, "numGRU,n", "Total Number of GRUs")
            .add(config_file, "config,c", "Path name of the config directory")
            .add(backup_server, "backup-server,b", "flag to denote if the server starting is a backup server")
            .add(server_mode, "server-mode,s", "enable server mode");
    }
};


behavior client(event_based_actor *self) {
    aout(self) << "Client Started\n";
    aout(self) << "Attempting To Connect To Server\n";


    return{
        [=](const std::string& arg) {
            aout(self) << "Client Received: " << arg << std::endl;
        }
    };
}


behavior server(event_based_actor *self) {
    aout(self) << "Server Started\n";
    return{
        [=](const std::string& arg) {
            aout(self) << "Server Received: " << arg << std::endl;
        }
    };
} 


void run_client(actor_system& system, const config& cfg) {
    scoped_actor self{system};
    
    auto server_actor = system.middleman().remote_actor("localhost", 4444);
    if(!server_actor) {aout(self) << "Failed To Connect To Server\n"; return;}
    aout(self) << "Connected To Server\n";

    self->send(*server_actor, "Hello From Client");
    
    // self->spawn(client);

}

void run_server(actor_system& system, const config& cfg) {
    scoped_actor self{system};

    int err;
    int port_number = 4444;

    aout(self) << "Attempting To Publish Actor On Port: " <<  port_number << "\n";
    auto server_actor = system.spawn(server);
    auto is_port = io::publish(server_actor, port_number);
    if(!is_port) {aout(self) << "Failed To Publish Actor On Port: " << port_number << "\n"; return;}

}


void caf_main(actor_system& system, const config& cfg) {
  auto f = cfg.server_mode ? run_server : run_client;
  f(system, cfg);
}


CAF_MAIN(io::middleman)