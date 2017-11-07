//
//  button system.cpp
//  Buttons
//
//  Created by Indi Kernick on 7/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "button system.hpp"

#include "collision component.hpp"
#include "collision categories.hpp"
#include "activation component.hpp"

void buttonSystem(Registry &registry) {
  auto view = registry.view<Collision, Activation>();
  for (EntityID entity : view) {
    const auto &collision = view.get<Collision>(entity);
    if (collision.category == getCategoryBit<Category::ButtonBase>()) {
      view.get<Activation>(entity).active = (
        collision.collidingCategory == getCategoryBit<Category::ButtonTop>()
      );
    }
  }
}
