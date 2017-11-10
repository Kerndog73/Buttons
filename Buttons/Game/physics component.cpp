//
//  physics component.cpp
//  Buttons
//
//  Created by Indi Kernick on 9/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "physics component.hpp"

#include "yaml helper.hpp"
#include "physics file.hpp"
#include "entity id map.hpp"
#include "../Libraries/Box2D/Dynamics/b2World.h"

PhysicsBody::PhysicsBody(const YAML::Node &node, const EntityIDmap &idMap, b2World &world) {
  Transform transform(node, idMap);
  body = loadBody(getChild(node, "body").Scalar(), world, transform);
}

PhysicsBody::~PhysicsBody() {
  if (body && body->GetWorld()) {
    body->GetWorld()->DestroyBody(body);
  }
}

PhysicsJoint::PhysicsJoint(const YAML::Node &node, const EntityIDmap &idMap, b2World &world) {
  
}
