# include "all.hpp"

behavior server(stateful_actor<server_state> *self) {
    aout(self) << "*\n*\n*\n ###### Server Starting Up ######\n\n";
    auto is_port = self->system().middleman().publish(self, 4444);
    if(!is_port) {aout(self) << "Failed To Publish Actor On Port: " << 4444 << "\n"; return {};}

    

    // Add self to the list of actors, the server is an actor like any other
    self->state.actor_list.push_back(self);


    self->set_down_handler([=](down_msg& dm) {
        aout(self) << "\n.\n.\n.\n.\n.\n";
        aout(self) << "Client Disconnected\n";
        aout(self) << "\n.\n.\n.\n";
        aout(self) << "Quitting Server\n";
        self->quit();
    });



    return{
        [=](caf::actor potential_client) {
            
            aout(self) << "\n-----Server Received Connection Request\n";
            self->state.actor_list.push_back(potential_client);
            aout(self) << "Server Added Client To List\n";
            aout(self) << "How Many Actors: " << self->state.actor_list.size() << "\n";

            // Send a message to the client
            self->send(potential_client, self->state.actor_list,  self);
        },


        [=](const std::string& arg, caf::actor return_addr) {
            aout(self) << "\n-----Server Received Message\n";
            // increment the number of messages received
            self->state.stats.num_messages_received++;
            // print the number of messages recieved
            aout(self) << "Number of Messages Received: " << self->state.stats.num_messages_received << "\n";

            
            // Sleep for something to do
            std::this_thread::sleep_for(std::chrono::seconds(3));


            // Send a message back to the clint
            self->send(return_addr, "Hello From Server", self);

            // increment the number of messages sent
            self->state.stats.num_messages_sent++;
            // print the number of messages sent
            aout(self) << "Number of Messages Sent: " << self->state.stats.num_messages_sent << "\n";
            aout(self) << "-----\n";
        }
    };
}