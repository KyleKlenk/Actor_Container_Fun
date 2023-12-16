#include "all.hpp"

void caf_main(actor_system& system, const config& cfg) {
    scoped_actor self{system};
    auto f = cfg.server_mode ? system.spawn(server) : system.spawn(client);
}


CAF_MAIN(io::middleman)