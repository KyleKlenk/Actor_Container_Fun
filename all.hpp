#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <caf/all.hpp>
#include "caf/io/all.hpp"

using namespace caf;

// Define some atoms

CAF_BEGIN_TYPE_ID_BLOCK(my_project, first_custom_type_id)

    CAF_ADD_ATOM(my_project, start_atom)

    // CAF_ADD_TYPE_ID(my_project, (std::vector<caf::actor>))

CAF_END_TYPE_ID_BLOCK(my_project)


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

struct statistics {
    int num_messages_sent = 0;
    int num_messages_received = 0;
};


struct server_state {
    std::vector<caf::actor> actor_list;
    int last_seen_actor_list_length = 0;

    // Statistics struct
    statistics stats;

};



behavior client(event_based_actor *self);
behavior server(stateful_actor<server_state> *self);