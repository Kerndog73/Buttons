//
//  player input system.cpp
//  Buttons
//
//  Created by Indi Kernick on 11/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "player input system.hpp"

#include <SDL2/SDL_events.h>
#include "player input component.hpp"

namespace {
  #define ASSIGN(MEMBER)                                                        \
    registry.view<PlayerInput>().each([down] (ECS::EntityID, PlayerInput &comp) {    \
      comp.MEMBER = down;                                                       \
    });
  
  bool handleKey(ECS::Registry &registry, const SDL_Scancode key, const bool down) {
    if (key == SDL_SCANCODE_LEFT || key == SDL_SCANCODE_A) {
      ASSIGN(left);
    } else if (key == SDL_SCANCODE_RIGHT || key == SDL_SCANCODE_D) {
      ASSIGN(right);
    } else if (key == SDL_SCANCODE_UP || key == SDL_SCANCODE_W) {
      ASSIGN(jump);
    } else if (key == SDL_SCANCODE_SPACE) {
      ASSIGN(action);
    } else {
      return false;
    }
    return true;
  }
  
  #undef ASSIGN
}

bool playerInputSystem(ECS::Registry &registry, const SDL_Event &event) {
  if (event.type == SDL_KEYDOWN) {
    return handleKey(registry, event.key.keysym.scancode, true);
  } else if (event.type == SDL_KEYUP) {
    return handleKey(registry, event.key.keysym.scancode, false);
  } else {
    return false;
  }
}
