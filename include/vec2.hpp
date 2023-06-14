#pragma once

#include <cmath>
#include <cstdint>
#include <ostream>

using std::uint32_t;

template <typename T> struct Vec2
{
  T x{}, y{};

  void normalize()
  {
    float magnitude{static_cast<float>(
        std::sqrt((this->x * this->x) + (this->y * this->y)))};
    if (magnitude != 0)
    {
      this->x /= magnitude;
      this->y /= magnitude;
    }
  }

  Vec2<float> normalized() const
  {
    float magnitude{static_cast<float>(
        std::sqrt((this->x * this->x) + (this->y * this->y)))};
    if (magnitude)
      return Vec2<float>{(this->x / magnitude), (this->y / magnitude)};
    return Vec2<float>{0};
  }

  float magnitude() const
  {
    return static_cast<float>(
        std::sqrt((this->x * this->x) + (this->y * this->y)));
  }

  bool zero() const { return (!this->x && !this->y); }

  void operator+=(const Vec2<T> &other)
  {
    this->x += other.x;
    this->y += other.y;
  }
  void operator-=(const Vec2<T> &other)
  {
    this->x -= other.x;
    this->y -= other.y;
  }
  void operator*=(const Vec2<T> &other)
  {
    this->x *= other.x;
    this->y *= other.y;
  }
  void operator/=(const Vec2<T> &other)
  {
    this->x /= other.x;
    this->y /= other.y;
  }

  void operator+=(const T &scalar)
  {
    this->x += scalar;
    this->y += scalar;
  }
  void operator-=(const T &scalar)
  {
    this->x -= scalar;
    this->y -= scalar;
  }
  void operator*=(const T &scalar)
  {
    this->x *= scalar;
    this->y *= scalar;
  }
  void operator/=(const T &scalar)
  {
    this->x /= scalar;
    this->y /= scalar;
  }
};

template <typename T>
Vec2<T> operator+(const Vec2<T> &first, const Vec2<T> &second)
{
  return Vec2<T>{first.x + second.x, first.y + second.y};
}
template <typename T>
Vec2<T> operator-(const Vec2<T> &first, const Vec2<T> &second)
{
  return Vec2<T>{first.x - second.x, first.y - second.y};
}
template <typename T>
Vec2<T> operator*(const Vec2<T> &first, const Vec2<T> &second)
{
  return Vec2<T>{first.x * second.x, first.y * second.y};
}
template <typename T>
Vec2<T> operator/(const Vec2<T> &first, const Vec2<T> &second)
{
  return Vec2<T>{first.x / second.x, first.y / second.y};
}

template <typename T> Vec2<T> operator+(const Vec2<T> &first, const T &scalar)
{
  return Vec2<T>{first.x + scalar, first.y + scalar};
}
template <typename T> Vec2<T> operator-(const Vec2<T> &first, const T &scalar)
{
  return Vec2<T>{first.x - scalar, first.y - scalar};
}
template <typename T> Vec2<T> operator*(const Vec2<T> &first, const T &scalar)
{
  return Vec2<T>{first.x * scalar, first.y * scalar};
}
template <typename T> Vec2<T> operator/(const Vec2<T> &first, const T &scalar)
{
  return Vec2<T>{first.x / scalar, first.y / scalar};
}

template <typename T>
bool operator==(const Vec2<T> &first, const Vec2<T> &second)
{
  return first.x == second.x && first.y == second.y;
}
template <typename T>
bool operator!=(const Vec2<T> &first, const Vec2<T> &second)
{
  return first.x != second.x || first.y != second.y;
}
template <typename T> bool operator==(const Vec2<T> &vec2, const T &scalar)
{
  return vec2.x == scalar && vec2.y == scalar;
}
template <typename T> bool operator!=(const Vec2<T> &vec2, const T &scalar)
{
  return vec2.x != scalar || vec2.y != scalar;
}

template <typename T> Vec2<T> operator-(const Vec2<T> &vec2)
{
  return Vec2{-vec2.x, -vec2.y};
}

template <typename T> std::ostream &operator<<(std::ostream &out, Vec2<T> vec2)
{
  out << "Vec2(" << vec2.x << ',' << vec2.y << ')';
  return out;
}
