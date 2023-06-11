#include "entity.hpp"

Entity::Entity(Vec2<float> position, Vec2<float> size)
    : mPosition{position}, mSize{size},
      mRenderRect{static_cast<int32_t>(position.x),
                  static_cast<int32_t>(position.y),
                  static_cast<int32_t>(size.x), static_cast<int32_t>(size.y)},
      mColor{0, 0, 0, 0} {}

void Entity::setColor(SDL_Color color) { this->mColor = color; }

void Entity::render(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
  SDL_RenderFillRect(renderer, &(this->mRenderRect));
}

void Entity::move(float delta, Vec2<float> offset) {
  this->mPosition.x += offset.x * delta;
  this->mPosition.y += offset.y * delta;
  this->mRenderRect.x = static_cast<int32_t>(this->mPosition.x);
  this->mRenderRect.y = static_cast<int32_t>(this->mPosition.y);
}

void Entity::setPosition(Vec2<float> position) {
  this->mPosition = position;
  this->mRenderRect.x = static_cast<int32_t>(this->mPosition.x);
  this->mRenderRect.y = static_cast<int32_t>(this->mPosition.y);
}

void Entity::setSize(Vec2<float> size) {
  this->mSize = size;
  this->mRenderRect.w = static_cast<int32_t>(this->mSize.x);
  this->mRenderRect.h = static_cast<int32_t>(this->mSize.y);
}

Vec2<float> Entity::getPosition() const { return this->mPosition; }
Vec2<float> Entity::getSize() const { return this->mSize; }
