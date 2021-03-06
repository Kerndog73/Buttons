//
//  door system.cpp
//  Buttons
//
//  Created by Indi Kernick on 12/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "door system.hpp"

#include "object types.hpp"
#include "door component.hpp"
#include "physics component.hpp"
#include "activation component.hpp"
#include <Box2D/Dynamics/b2Fixture.h>

void doorSystem(ECS::Registry &registry) {
  auto view = registry.view<Door, PhysicsBody, Activation>();
  for (const ECS::EntityID entity : view) {
    const bool active = isActive(view.get<Activation>(entity).state);
    b2Body *const body = view.get<PhysicsBody>(entity).body;
    b2Fixture *const fixture = body->GetFixtureList();
    fixture->SetSensor(active);
    fixture->SetUserData(
      active
      ? B2::getObjectTypeUserData<B2::ObjT::Sensor>()
      : B2::getObjectTypeUserData<B2::ObjT::Ground>()
    );
  }
}
