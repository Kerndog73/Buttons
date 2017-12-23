//
//  laser sprite rendering system.cpp
//  Buttons
//
//  Created by Indi Kernick on 22/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "laser sprite rendering system.hpp"

#include "b2 glm cast.hpp"
#include "physics component.hpp"
#include "activation component.hpp"
#include "laser rendering component.hpp"

namespace {
  // rotates by 90 degrees
  glm::vec2 perp(const glm::vec2 v) {
    return {-v.y, v.x};
  }
  glm::vec3 setDepth(const glm::vec2 pos, const float depth) {
    return glm::vec3(pos.x, pos.y, depth);
  }
}

size_t countLaserSprites(Registry &registry) {
  return registry.view<LaserRendering>().size();
}

void writeLaserSprites(
  Registry &registry,
  const Spritesheet &sheet,
  QuadIter quadIter
) {
  static const Unpack::SpriteID LASER_ID = sheet.getIDfromName("laser");
  static const TexCoords LASER_COORDS = lookupTexCoords(sheet, LASER_ID);
  
  const auto view = registry.view<LaserRendering, PhysicsRayCast, Activation>();
  
  for (const EntityID entity : view) {
    Quad &quad = *quadIter;
    
    if (isActive(view.get<Activation>(entity).state)) {
      const float depth = view.get<LaserRendering>(entity).depth;
      const PhysicsRayCast &rayCast = view.get<PhysicsRayCast>(entity);
      const glm::vec2 start = castToGLM(rayCast.start);
      const glm::vec2 startToEnd = (castToGLM(rayCast.end) - start);
      const glm::vec2 end = startToEnd * rayCast.closestFraction + start;
      const glm::vec2 toTopLeft = 0.5f * glm::normalize(perp(startToEnd));
      
      quad[0].pos = setDepth(start + toTopLeft, depth);
      quad[1].pos = setDepth(start - toTopLeft, depth);
      quad[2].pos = setDepth(end - toTopLeft, depth);
      quad[3].pos = setDepth(end + toTopLeft, depth);
    } else {
      for (size_t v = 0; v != 4; ++v) {
        quad[v].pos = glm::vec3(0.0f, 0.0f, -1.0f);
      }
    }
    
    setTexCoords(quad, LASER_COORDS);
    
    ++quadIter;
  }
}