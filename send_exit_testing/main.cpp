#include <iostream>
#include "caf/all.hpp"
using namespace caf;

behavior child(event_based_actor* self) {
  return {
    [=](std::string& what) {
      aout(self) << "Hello " << what << "!" << std::endl;
      self->send_exit(self, exit_reason::unknown);
      self-
    }
  };
}

behavior parent(event_based_actor* self) {
      self->set_down_handler([=](const down_msg& dm) {
        aout(self) << "\n\n ********** DOWN HANDLER ********** \n";
        aout(self) << "Lost Connection With A Connected Actor\n";
        aout(self) << "Reason: " << to_string(dm.reason) << "\n";
    });

    self->set_error_handler([=](const error& err) {
        aout(self) << "\n\n ********** ERROR HANDLER ********** \n";
        aout(self) << "Error: " << to_string(err) << "\n";
    });

    self->set_exit_handler([=](const exit_msg& em) {
        aout(self) << "\n\n ********** EXIT HANDLER ********** \n";
        aout(self) << "Exit Reason: " << to_string(em.reason) << "\n";
    });


  return {
    [=](std::string& what) {
      auto child_actor = self->spawn(child);
      self->monitor(child_actor);
      self->send(child_actor, what);
    }
  };
}




void caf_main(actor_system& system) {
  scoped_actor self{system};
  auto p = system.spawn(parent);
  self->send(p, std::string("World"));
}

CAF_MAIN()