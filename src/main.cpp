#include <SDL.h>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

#include "entity.hpp"
#include "vec2.hpp"

using std::uint32_t;

constexpr int32_t window_width_v{1280};
constexpr int32_t window_height_v{720};
constexpr int32_t padding_v{200};
constexpr float blocksize_v{50.0f};
constexpr float player_velocity_v{0.5f};

bool playerUp{}, playerDown{}, playerLeft{}, playerRight{};
Vec2<float> playerDirection{};

struct InputSystem
{
  std::set<SDL_Keycode> pressedKeys;
  std::set<SDL_Keycode> releasedKeys;
  std::set<SDL_Keycode> heldKeys;

  void reset()
  {
    this->pressedKeys.clear();
    this->releasedKeys.clear();
  }
} gInput{};

struct SdlDeleter
{
  void operator()(SDL_Renderer *renderer) const
  {
    SDL_DestroyRenderer(renderer);
  }
  void operator()(SDL_Window *window) const { SDL_DestroyWindow(window); }
};

namespace
{
std::random_device dev{};
std::mt19937 rng{dev()};
} // namespace

int32_t randomInt(int32_t start, int32_t end)
{
  std::uniform_int_distribution<int32_t> idist{start, end};
  return idist(rng);
}

float randomFloat(float start, float end)
{
  std::uniform_real_distribution<float> fdist{start, end};
  return fdist(rng);
}

SDL_Window *createSdlWindow(const char *title, Vec2<int32_t> size)
{
  SDL_Window *window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       size.x, size.y, SDL_WINDOW_RESIZABLE);
  if (!window)
  {
    std::stringstream message{};
    message << "Failed to create a window, " << SDL_GetError();
    throw std::runtime_error(message.str());
  }

  return window;
}

SDL_Renderer *createSdlRenderer(SDL_Window *window, uint32_t flags)
{
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, flags);
  if (!renderer)
  {
    std::stringstream message{};
    message << "Failed to create a renderer, " << SDL_GetError();
    throw std::runtime_error(message.str());
  }

  return renderer;
}

Entity player{Vec2<float>{randomFloat(padding_v, window_width_v - padding_v),
                          randomFloat(padding_v, window_height_v - padding_v)},
              Vec2<float>{blocksize_v, blocksize_v}};
Entity target{Vec2<float>{randomFloat(padding_v, window_width_v - padding_v),
                          randomFloat(padding_v, window_height_v - padding_v)},
              Vec2<float>{blocksize_v, blocksize_v}};

void playerUpdate(float delta)
{
  if (gInput.heldKeys.contains(SDLK_w))
  {
    playerDirection.y = -1;
    playerDown = false;
    playerUp = true;
  }

  if (gInput.heldKeys.contains(SDLK_s))
  {
    playerDirection.y = 1;
    playerUp = false;
    playerDown = true;
  }

  if (gInput.heldKeys.contains(SDLK_a))
  {
    playerDirection.x = -1;
    playerRight = false;
    playerLeft = true;
  }
  if (gInput.heldKeys.contains(SDLK_d))
  {
    playerDirection.x = 1;
    playerLeft = false;
    playerRight = true;
  }

  if (!gInput.heldKeys.contains(SDLK_d) && !gInput.heldKeys.contains(SDLK_a))
  {
    playerDirection.x = 0;
    playerLeft = playerRight = false;
  }
  if (!gInput.heldKeys.contains(SDLK_w) && !gInput.heldKeys.contains(SDLK_s))
  {
    playerDirection.y = 0;
    playerUp = playerDown = false;
  }
  Vec2<float> playerDirectionNormalized = playerDirection.normalized();

  if (playerRight)
  {
    if ((player.getPosition().x + player.getSize().x) < window_width_v)
      player.move(
          delta,
          Vec2<float>{playerDirectionNormalized.x * player_velocity_v, 0});
  }
  if (playerLeft)
  {
    if (player.getPosition().x > 0)
      player.move(
          delta,
          Vec2<float>{playerDirectionNormalized.x * player_velocity_v, 0});
  }
  if (playerUp)
  {
    if (player.getPosition().y > 0)
      player.move(delta, Vec2<float>{0, playerDirectionNormalized.y *
                                            player_velocity_v});
  }
  if (playerDown)
  {
    if ((player.getPosition().y + player.getSize().y) < window_height_v)
      player.move(delta, Vec2<float>{0, playerDirectionNormalized.y *
                                            player_velocity_v});
  }
}

int main(int argc, char *argv[])
{
  std::unique_ptr<SDL_Window, SdlDeleter> gWindow{nullptr};
  std::unique_ptr<SDL_Renderer, SdlDeleter> gRenderer{nullptr};

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cout << "Failed to initialize SDL2, " << SDL_GetError() << std::endl;
    return 1;
  }

  try
  {
    gWindow = std::unique_ptr<SDL_Window, SdlDeleter>{
        createSdlWindow("Game", Vec2<int32_t>{1280, 720})};
    gRenderer = std::unique_ptr<SDL_Renderer, SdlDeleter>{
        createSdlRenderer(gWindow.get(), SDL_RENDERER_ACCELERATED)};
  }
  catch (std::exception &e)
  {
    std::cout << e.what() << std::endl;
    return 1;
  }

  SDL_SetRenderDrawBlendMode(gRenderer.get(), SDL_BLENDMODE_BLEND);

  bool running{true};
  SDL_Event event{};

  target.setColor(SDL_Color{78, 255, 45, 255});

  auto start{SDL_GetTicks()};

  while (running)
  {
    gInput.reset();

    auto distance = std::sqrt(static_cast<float>(
        std::pow((player.getPosition().x - target.getPosition().x), 2) +
        std::pow((player.getPosition().y - target.getPosition().y), 2)));

    if (distance <= 255 && distance >= 0)
      player.setColor(SDL_Color{
          .r = 245, .g = 66, .b = 78, .a = static_cast<uint8_t>(distance)});
    else
      player.setColor(SDL_Color{.r = 245, .g = 66, .b = 78, .a = 255});

    auto current{SDL_GetTicks()};
    auto delta{current - start};
    SDL_SetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "letterbox");
    SDL_RenderSetLogicalSize(gRenderer.get(), window_width_v, window_height_v);

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_KEYDOWN:
        gInput.pressedKeys.insert(event.key.keysym.sym);
        gInput.heldKeys.insert(event.key.keysym.sym);
        break;
      case SDL_KEYUP:
        gInput.releasedKeys.insert(event.key.keysym.sym);
        gInput.heldKeys.erase(gInput.heldKeys.erase(event.key.keysym.sym));
        break;
      }
    }

    playerUpdate(delta);

    SDL_SetRenderDrawColor(gRenderer.get(), 30, 30, 30, 255);
    SDL_RenderClear(gRenderer.get());

    player.render(gRenderer.get());

    target.render(gRenderer.get());

    SDL_RenderPresent(gRenderer.get());

    start = current;
  }

  return EXIT_SUCCESS;
}
