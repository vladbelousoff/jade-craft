#pragma once

#include <SDL2/SDL.h>
#include <functional>
#include <utility>

struct IRenderContext
{
   virtual ~IRenderContext() = default;

   virtual auto get_window_flags() -> SDL_WindowFlags = 0;

   virtual auto init(SDL_Window* window) -> bool = 0;
   virtual void term(SDL_Window* window) = 0;

   virtual void draw_scene(SDL_Window* window, const std::function<void()>& callback) = 0;

   virtual auto get_drawable_size(SDL_Window* window) -> std::pair<int, int> = 0;
   virtual void viewport(int x, int y, int w, int h) = 0;

   virtual void clear(float r, float g, float b, float a) = 0;
};
