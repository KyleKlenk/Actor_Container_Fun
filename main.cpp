#include <iostream>
#include <chrono>
#include <thread>
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


struct client_state {
    strong_actor_ptr server;
};

behavior client(event_based_actor *self) {
    self->set_down_handler([=](down_msg& dm) {

        aout(self) << "\n.\n.\n.\n.\n.\n";
        aout(self) << "Server Disconnected\n";
        aout(self) << "\n.\n.\n.\n";
        aout(self) << "Quitting Client\n";
        self->quit();

    });


    aout(self) << "Client Started\n";
    aout(self) << "Attempting To Connect To Server\n";
    auto server = self->system().middleman().remote_actor("localhost", 4444);
    if(!server) {aout(self) << "Failed To Connect To Server\n"; return {};}
    self->monitor(*server);
    aout(self) << "Connected To Server\n";
    self->send(*server, "Hello From Client", self);

    return{
        [=](const std::string& arg, caf::actor return_addr) {

            aout(self) << "Client Received: " << arg << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(5));
            
            self->send(return_addr, "Hello From Client", self);

        }
    };
}

struct server_state {
    std::vector<caf::actor> actor_list;
    int last_seen_actor_list_length = 0;
};


behavior server(stateful_actor<server_state> *self) {
    aout(self) << "Server Started\n";
    self->state.actor_list.push_back(self);

    // self->set_down_handler([=](down_msg& dm) {
    //     aout(self) << "\n.\n.\n.\n.\n.\n";
    //     aout(self) << "Client Disconnected\n";
    //     aout(self) << "\n.\n.\n.\n";
    //     aout(self) << "Quitting Server\n";
    //     self->quit();
    // });


    return{
        [=](caf::actor potential_client) {
            aout(self) << "Server Received Connection Request\n";
            self->state.actor_list.push_back(potential_client);
        },


        [=](const std::string& arg, caf::actor return_addr) {
            aout(self) << "Server Received: " << arg << std::endl;
            if (self->state.actor_list.size() != self->state.last_seen_actor_list_length) {
                self->state.last_seen_actor_list_length++;

                self->send(self->state.actor_list[self->state.actor_list.size() ])
            }

            std::this_thread::sleep_for(std::chrono::seconds(5));
            self->send(return_addr, "Hello From Server", self);
        }
    };
} 


void run_client(actor_system& system, const config& cfg) {
    scoped_actor self{system};
    
    auto server_actor = system.middleman().remote_actor("localhost", 4444);

    self->spawn(client);

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