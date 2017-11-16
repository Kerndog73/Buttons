//
//  moving platform rendering system.hpp
//  Buttons
//
//  Created by Indi Kernick on 16/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef moving_platform_rendering_system_hpp
#define moving_platform_rendering_system_hpp

#include "entity registry.hpp"

extern "C" struct NVGcontext;

void movingPlatformRenderingSystem(Registry &, NVGcontext *);

#endif
