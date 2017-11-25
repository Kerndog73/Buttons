//
//  lock init.cpp
//  Buttons
//
//  Created by Indi Kernick on 25/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "lock init.hpp"

#include "yaml helper.hpp"
#include "player keys component.hpp"

void LockInit::init(Lock &lock, const YAML::Node &node) {
  lock.index = getChild(node, "index").as<uint8_t>();
  if (lock.index >= MAX_KEYS) {
    throw std::runtime_error("Lock index out of range");
  }
}
