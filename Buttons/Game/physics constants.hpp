//
//  physics constants.hpp
//  Buttons
//
//  Created by Indi Kernick on 7/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef physics_constants_hpp
#define physics_constants_hpp

#include <Box2D/Common/b2Math.h>

const b2Vec2 GRAVITY = {0.0f, -9.80665f};
constexpr int32 VELOCITY_ITER = 16;
constexpr int32 POSITION_ITER = 8;

constexpr float DEBUG_DRAW_STROKE_WIDTH = 1.0f / 32.0f;

constexpr bool ENABLE_SHAPE_RENDER = true;
constexpr bool ENABLE_JOINT_RENDER = true;
constexpr bool ENABLE_AABB_RENDER = false;
constexpr bool ENABLE_PAIR_RENDER = false;
constexpr bool ENABLE_CENTER_OF_MASS_RENDER = false;

#endif
