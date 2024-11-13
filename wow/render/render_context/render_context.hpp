#pragma once

#include <SDL2/SDL.h>
#include <utility>

struct IRenderContext
{
   virtual auto get_window_flags() -> SDL_WindowFlags = 0;

   virtual auto init(SDL_Window* window) -> bool = 0;
   virtual void term(SDL_Window* window) = 0;
   virtual void swap(SDL_Window* window) = 0;

   virtual auto get_drawable_size(SDL_Window* window) -> std::pair<int, int> = 0;
   virtual void viewport(int x, int y, int w, int h) = 0;

   virtual void clear() = 0;
   virtual void clear_color(float r, float g, float b, float a) = 0;
};
