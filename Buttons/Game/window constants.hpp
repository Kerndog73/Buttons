//
//  window constants.hpp
//  Buttons
//
//  Created by Indi Kernick on 8/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef window_constants_hpp
#define window_constants_hpp

#include <Simpleton/SDL/window.hpp>

const glm::ivec2 DEFAULT_WINDOW_PIXEL_SIZE = {1280, 720};

constexpr bool WINDOW_RESIZABLE = true;
constexpr bool WINDOW_VSYNC = true;
constexpr bool WINDOW_OPENGL = true;

const SDL::Window::Desc WINDOW_DESC = {
  "Buttons",
  DEFAULT_WINDOW_PIXEL_SIZE,
  WINDOW_RESIZABLE,
  WINDOW_OPENGL
};

#endif
