//
//  laser detector system.cpp
//  Buttons
//
//  Created by Indi Kernick on 20/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "laser detector system.hpp"

#include "physics component.hpp"
#include "activation component.hpp"
#include "laser detector component.hpp"

void laserDetectorSystem(Registry &registry) {
  auto detectors = registry.view<Activation, LaserDetector>();
  const auto emitters = registry.view<PhysicsRayCast>();
  for (const EntityID entity : detectors) {
    const EntityID emitter = detectors.get<LaserDetector>(entity).emitter;
    const float fraction = emitters.get(emitter).closestFraction;
    Activation::State &activeState = detectors.get<Activation>(entity).state;
    if (fraction >= 0.99f) {
      activate(activeState);
    } else {
      deactivate(activeState);
    }
  }
}
