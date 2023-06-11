#pragma once

#include "vec2.hpp"
#include <SDL.h>
#include <cstdint>

using std::int32_t;
using std::uint32_t;

class Entity
{
public:
  Entity(Vec2<float> position, Vec2<float> size);
  void setColor(SDL_Color color);
  void render(SDL_Renderer *renderer);
  void move(float delta, Vec2<float> offset);
  void setPosition(Vec2<float> position);
  void setSize(Vec2<float> size);

  Vec2<float> getPosition() const;
  Vec2<float> getSize() const;

protected:
  Vec2<float> mPosition{}, mSize{};
  SDL_Rect mRenderRect{};
  SDL_Color mColor{};
};
