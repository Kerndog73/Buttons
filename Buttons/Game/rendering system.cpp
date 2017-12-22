//
//  rendering system.cpp
//  Buttons
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "rendering system.hpp"

#include <fstream>
#include "b2 glm cast.hpp"
#include "physics component.hpp"
#include "animation component.hpp"
#include "transform component.hpp"
#include "activation component.hpp"
#include <Simpleton/OpenGL/uniforms.hpp>
#include "laser rendering component.hpp"
#include <Simpleton/Platform/system info.hpp>
#include <Simpleton/OpenGL/attrib pointer.hpp>
#include "anim sprite rendering component.hpp"
#include "static sprite rendering component.hpp"

namespace {
  constexpr GLint POS_ID = 0;
  constexpr GLint TEX_COORD_ID = 1;
  
  ///Number of indicies that make up a quad
  constexpr size_t QUAD_INDICIES = 6;
  ///Number of verticies that make up a quad
  constexpr size_t QUAD_VERTS = 4;
  ///Byte size of a quad in a GL_ARRAY_BUFFER
  constexpr size_t QUAD_ATTR_SIZE = (sizeof(PosType) + sizeof(TexCoordType)) * QUAD_VERTS;
  ///Byte size of a quad in a GL_ELEMENT_ARRAY_BUFFER
  constexpr size_t QUAD_ELEM_SIZE = sizeof(ElemType) * QUAD_INDICIES;
  
  constexpr auto *enablePos = GL::enable<POS_ID, PosType>;
  constexpr auto *enableTexCoord = GL::enable<TEX_COORD_ID, TexCoordType>;
  
  constexpr auto *posPointer = GL::attribPointer<POS_ID, PosType>;
  constexpr auto *texCoordPointer = GL::attribPointer<TEX_COORD_ID, TexCoordType>;
}

void RenderingSystem::init() {
  const std::string path = Platform::getResDir() + "sprites.";
  sheet = Unpack::makeSpritesheet(path + "atlas", path + "png");
  const Unpack::Image &image = sheet.getImage();
  const GL::Image2D glImage = {image.data(), image.width(), image.height()};
  
  GL::TexParams2D texParams;
  texParams.setWrap(GL_CLAMP_TO_EDGE);
  texParams.setFilter(GL_NEAREST);
  glActiveTexture(GL_TEXTURE0);
  CHECK_OPENGL_ERROR();
  texture = GL::makeTexture2D(glImage, texParams);
  
  vertArray = GL::makeVertexArray();
  
  std::ifstream vertFile(Platform::getResDir() + "sprite shader.vert");
  assert(vertFile.is_open());
  std::ifstream fragFile(Platform::getResDir() + "sprite shader.frag");
  assert(fragFile.is_open());
  
  std::cout << "Creating vertex shader\n";
  GL::Shader vertShader = GL::makeShader(GL_VERTEX_SHADER, vertFile);
  std::cout << "Creating fragment shader\n";
  GL::Shader fragShader = GL::makeShader(GL_FRAGMENT_SHADER, fragFile);
  
  program = GL::makeShaderProgram(vertShader, fragShader);
  
  viewProjLoc = program.getUniformLoc("viewProj");
  texLoc = program.getUniformLoc("tex");
  
  program.use();
  GL::setUniform(texLoc, 0);
  GL::unuseProgram();
}

void RenderingSystem::quit() {
  elemBuf = nullptr;
  arrayBuf = nullptr;
  vertArray = nullptr;
  texture = nullptr;
}

const Unpack::Spritesheet &RenderingSystem::getSheet() const {
  return sheet;
}

void RenderingSystem::onLevelLoad(Registry &registry) {
  numQuads
  = registry.view<StaticSpriteRendering>().size()
  + registry.view<AnimSpriteRendering>().size()
  + registry.view<LaserRendering>().size();
  
  fillIndicies(numQuads);
  verts.resize(numQuads * QUAD_VERTS);
  
  vertArray.bind();
  program.use();
  
  arrayBuf = GL::makeArrayBuffer(numQuads * QUAD_ATTR_SIZE, GL_DYNAMIC_DRAW);
  elemBuf = GL::makeElementBuffer(indicies.data(), numQuads * QUAD_ELEM_SIZE);
  
  posPointer(sizeof(PosType) + sizeof(TexCoordType), 0);
  enablePos();
  texCoordPointer(sizeof(PosType) + sizeof(TexCoordType), sizeof(PosType));
  enableTexCoord();
  
  GL::unbindVertexArray();
}

void RenderingSystem::render(Registry &registry, const glm::mat3 &viewProj) {
  size_t spriteIndex = 0;
  animSprites(registry, spriteIndex);
  staticSprites(registry, spriteIndex);
  laserSprites(registry, spriteIndex);
  
  vertArray.bind();
  GL::setUniform(viewProjLoc, viewProj);
  fillVBOs();
  if (!program.validate()) {
    std::cerr << "Failed to validate program\n";
    std::cerr << "Shader program info log:\n" << program << '\n';
  }
  glDrawElements(
    GL_TRIANGLES,
    static_cast<GLsizei>(QUAD_INDICIES * numQuads),
    GL::TypeEnum<ElemType>::type,
    0
  );
  CHECK_OPENGL_ERROR();
  GL::unbindVertexArray();
}

void RenderingSystem::fillIndicies(const size_t minQuads) {
  if (indicies.size() < minQuads * QUAD_INDICIES) {
    ElemType index = indicies.size() / QUAD_INDICIES * QUAD_VERTS;
    const ElemType lastIndex = minQuads * QUAD_VERTS;
    for (; index != lastIndex; index += 4) {
      indicies.push_back(index + 0);
      indicies.push_back(index + 1);
      indicies.push_back(index + 2);
      indicies.push_back(index + 2);
      indicies.push_back(index + 3);
      indicies.push_back(index + 0);
    }
  }
}

void RenderingSystem::fillVBOs() {
  const size_t vertsSize = sizeof(Vertex) * verts.size();
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertsSize, verts.data());
  CHECK_OPENGL_ERROR();
  
  const size_t indiciesSize = sizeof(ElemType) * QUAD_INDICIES * numQuads;
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indiciesSize, indicies.data());
  CHECK_OPENGL_ERROR();
}

namespace {
  Unpack::SpriteID getFrame(
    const float progress,
    const Unpack::SpriteID startFrame,
    const Unpack::SpriteID numFrames
  ) {
    constexpr float MUL = 0.9999f;
    return startFrame + progress * numFrames * MUL;
  }
  
  glm::vec3 mulPos(const glm::mat3 &mat, const glm::vec2 pos, const float depth) {
    const glm::vec3 newPos = mat * glm::vec3(pos.x, pos.y, 1.0f);
    return {newPos.x, newPos.y, depth};
  }
}

glm::vec2 RenderingSystem::getSheetSize() const {
  return {sheet.getImage().width(), sheet.getImage().height()};
}

RenderingSystem::TexCoords RenderingSystem::getTexCoords(
  const Unpack::SpriteID sprite
) {
  const glm::vec2 sheetSize = getSheetSize();
  const Unpack::RectPx rect = sheet.getSprite(sprite);
  TexCoords coords;
  coords.bottomLeft = glm::vec2(rect.x, rect.y) / sheetSize;
  coords.topRight = coords.bottomLeft + glm::vec2(rect.w, rect.h) / sheetSize;
  return coords;
}

void RenderingSystem::setPositions(
  const size_t index,
  const glm::mat3 &world,
  const float depth,
  const glm::vec2 offset,
  const glm::vec2 scale
) {
  verts[index + 0].pos = mulPos(world, offset + scale * glm::vec2(0.0f, 0.0f), depth);
  verts[index + 1].pos = mulPos(world, offset + scale * glm::vec2(1.0f, 0.0f), depth);
  verts[index + 2].pos = mulPos(world, offset + scale * glm::vec2(1.0f, 1.0f), depth);
  verts[index + 3].pos = mulPos(world, offset + scale * glm::vec2(0.0f, 1.0f), depth);
}

void RenderingSystem::setTexCoords(
  const size_t index,
  const TexCoords coords
) {
  verts[index + 0].texCoord = {coords.bottomLeft.x, coords.topRight.y};
  verts[index + 1].texCoord = coords.topRight;
  verts[index + 2].texCoord = {coords.topRight.x, coords.bottomLeft.y};
  verts[index + 3].texCoord = coords.bottomLeft;
}

void RenderingSystem::staticSprites(Registry &registry, size_t &spriteIndex) {
  const auto view = registry.view<StaticSpriteRendering, Transform>();
  
  for (const EntityID entity : view) {
    const StaticSpriteRendering anim = view.get<StaticSpriteRendering>(entity);
    
    setPositions(spriteIndex, getMat3(view.get<Transform>(entity)), anim.depth);
    setTexCoords(spriteIndex, getTexCoords(anim.sprite));
    
    spriteIndex += 4;
  }
}

void RenderingSystem::animSprites(Registry &registry, size_t &spriteIndex) {
  const auto view = registry.view<AnimSpriteRendering, Animation, Transform>();
  
  for (const EntityID entity : view) {
    const float progress = view.get<Animation>(entity).progress;
    const AnimSpriteRendering anim = view.get<AnimSpriteRendering>(entity);
    const Unpack::SpriteID frame = getFrame(progress, anim.sprite, anim.frames);
    
    setPositions(spriteIndex, getMat3(view.get<Transform>(entity)), anim.depth, anim.offset, anim.scale);
    setTexCoords(spriteIndex, getTexCoords(frame));
    
    spriteIndex += 4;
  }
}

namespace {
  // rotates by 90 degrees
  glm::vec2 perp(const glm::vec2 v) {
    return {-v.y, v.x};
  }
  glm::vec3 setDepth(const glm::vec2 pos, const float depth) {
    return glm::vec3(pos.x, pos.y, depth);
  }
}

void RenderingSystem::laserSprites(Registry &registry, size_t &spriteIndex) {
  static const Unpack::SpriteID LASER_ID = sheet.getIDfromName("laser");
  const auto view = registry.view<LaserRendering, PhysicsRayCast, Activation>();
  
  for (const EntityID entity : view) {
    if (isActive(view.get<Activation>(entity).state)) {
      const float depth = view.get<LaserRendering>(entity).depth;
      const PhysicsRayCast &rayCast = view.get<PhysicsRayCast>(entity);
      const glm::vec2 start = castToGLM(rayCast.start);
      const glm::vec2 startToEnd = (castToGLM(rayCast.end) - start);
      const glm::vec2 end = startToEnd * rayCast.closestFraction + start;
      const glm::vec2 toTopLeft = 0.5f * glm::normalize(perp(startToEnd));
      
      verts[spriteIndex + 0].pos = setDepth(start + toTopLeft, depth);
      verts[spriteIndex + 1].pos = setDepth(start - toTopLeft, depth);
      verts[spriteIndex + 2].pos = setDepth(end - toTopLeft, depth);
      verts[spriteIndex + 3].pos = setDepth(end + toTopLeft, depth);
    } else {
      for (size_t i = 0; i != 4; ++i) {
        verts[spriteIndex + i].pos = glm::vec3(0.0f, 0.0f, -1.0f);
      }
    }
    
    setTexCoords(spriteIndex, getTexCoords(LASER_ID));
    
    spriteIndex += 4;
  }
}