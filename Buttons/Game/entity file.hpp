//
//  entity file.hpp
//  Buttons
//
//  Created by Indi Kernick on 10/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef entity_file_hpp
#define entity_file_hpp

#include <yaml-cpp/yaml.h>
#include "entity id map.hpp"
#include "entity registry.hpp"

void loadEntity(const std::string &, const YAML::Node &, EntityIDmap &, Registry &);

#endif
