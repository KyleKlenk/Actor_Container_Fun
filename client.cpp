#include "all.hpp"

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


    aout(self) << "Client Started -- Attempting Connection\n";
    auto server = self->system().middleman().remote_actor("172.17.0.3", 4444);
    if(!server) {aout(self) << "Failed To Connect To Server\n"; return {};}
    self->monitor(*server);
    aout(self) << "Connected To Server\n";
    self->send(*server, self);

    return{
        [=](const std::string& arg, caf::actor return_addr) {

            

            aout(self) << "Client Received: " << arg << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(5));
            
            self->send(return_addr, "Hello From Client", self);

        },

        [=](std::vector(caf::actor x)) {
            
        }


    };
}