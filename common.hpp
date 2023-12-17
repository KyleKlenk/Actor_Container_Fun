#pragma once

#include "caf/all.hpp"
#include "caf/io/all.hpp"
#include "/home/kyle/Actor_Container_Fun/tomlplusplus/include/toml++/toml.h"


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

