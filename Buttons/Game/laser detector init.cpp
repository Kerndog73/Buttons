//
//  laser detector init.cpp
//  Buttons
//
//  Created by Indi Kernick on 19/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "laser detector init.hpp"

#include <Simpleton/ECS/entity id map.hpp>

void LaserDetectorInit::init(LaserDetector &comp, const json &node, const ECS::EntityIDmap &idMap) {
  comp.emitter = idMap.getEntityFromID(node.at("emitter").get<ECS::ClientEntityID>());
}
