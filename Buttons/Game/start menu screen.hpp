//
//  start menu screen.hpp
//  Buttons
//
//  Created by Indi Kernick on 26/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef start_menu_screen_hpp
#define start_menu_screen_hpp

#include "screen.hpp"
#include "entity registry.hpp"
#include <Simpleton/Camera 2D/camera.hpp>

extern "C" union SDL_Event;

class StartMenuScreen final : public Screen {
public:
  StartMenuScreen() = default;
  
  void init(RenderingSystem &) override;
  void quit() override;
  void input(const SDL_Event &) override;
  void update(float) override;
  void render(float, float) override;

private:
  Cam2D::Camera camera;
  RenderingSystem *rendering;
  Registry registry;
  bool startGame = false;
};

#endif
