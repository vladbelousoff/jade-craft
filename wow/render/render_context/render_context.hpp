#pragma once

#include <SDL2/SDL.h>

struct IRenderContext
{
   virtual auto get_window_flags() -> SDL_WindowFlags = 0;

   virtual auto init(SDL_Window* window) -> bool = 0;
   virtual void term(SDL_Window* window) = 0;
   virtual void swap(SDL_Window* window) = 0;

   virtual void get_drawable_size(SDL_Window* window, int& w, int& h) = 0;
};
