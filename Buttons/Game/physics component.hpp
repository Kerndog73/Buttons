//
//  physics body component.hpp
//  Buttons
//
//  Created by Indi Kernick on 7/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef physics_body_component_hpp
#define physics_body_component_hpp

#include <glm/vec2.hpp>
#include "entity registry.hpp"
#include "../Libraries/Box2D/Dynamics/b2Body.h"
#include "../Libraries/Box2D/Dynamics/Joints/b2Joint.h"

struct PhysicsBody {
  PhysicsBody() = default;
  PhysicsBody(PhysicsBody &&);
  PhysicsBody &operator=(PhysicsBody &&);
  ~PhysicsBody();
  
  b2Body *body = nullptr;
  glm::vec2 scale;
};

struct PhysicsJoint {
  b2Joint *joint = nullptr;
};

#endif
