//
//  door system.cpp
//  Buttons
//
//  Created by Indi Kernick on 12/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "door system.hpp"

#include "door component.hpp"
#include "physics component.hpp"
#include "activation component.hpp"
#include "../Libraries/Box2D/Dynamics/b2Fixture.h"

void doorSystem(Registry &registry) {
  auto view = registry.view<Door, PhysicsBody, Activation>();
  for (EntityID entity : view) {
    view.get<PhysicsBody>(entity).body->GetFixtureList()->SetSensor(
      view.get<Activation>(entity).state == Activation::State::ACTIVE
    );
  }
}
