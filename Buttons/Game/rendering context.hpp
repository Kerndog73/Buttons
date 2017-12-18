//
//  rendering context.hpp
//  Catastrophe
//
//  Created by Indi Kernick on 4/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef rendering_context_hpp
#define rendering_context_hpp

#include <glm/vec2.hpp>

extern "C" struct SDL_Window;
extern "C" struct SDL_Renderer;

class RenderingContext {
public:
  RenderingContext() = default;
  
  void init(SDL_Window *, bool);
  void quit();
  
  void preRender();
  void postRender();
  
  glm::ivec2 getFrameSize() const;
  SDL_Window *getWindow() const;
  SDL_Renderer *getRenderer() const;
  
private:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
};

#endif
