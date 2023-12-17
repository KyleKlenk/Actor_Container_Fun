#pragma once

#include "caf/all.hpp"
#include "caf/io/all.hpp"
#include "/root/Actor_Container_Fun/tomlplusplus/include/toml++/toml.h"
#include <iostream>

using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(app, first_custom_type_id)
  CAF_ADD_ATOM(app, new_connection)
CAF_END_TYPE_ID_BLOCK(app)

std::optional<toml::table> parse_config(const std::string& filename) {
  try {
    return toml::parse_file(filename);
  } catch (const toml::parse_error& err) {
    return std::nullopt;
  }
}


behavior listener(event_based_actor* self, actor server_actor) {
  while(true) {
    std::string name;
    aout(self) << "Enter your name: ";
    std::getline(std::cin, name);
    self->send(server_actor, name);
    std::this_thread::sleep_for(std::chrono::seconds(1));

  }
  return {};

}

