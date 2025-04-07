#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_render.h>
#include <m_pd.h>

#include "multy.h"

#define WIDTH 600
#define HEIGHT 600

void render(SDL_Renderer *renderer, uint step) {
  SDL_Rect rect = {step * 50 % WIDTH, 0, 50, 50};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 255);
  SDL_RenderFillRect(renderer, &rect);
  SDL_RenderPresent(renderer);
}

void *run(void *x) {
  t_multy *m = (t_multy *)x;

  post("multy~ • Creating renderer...");

  m->renderer = SDL_CreateRenderer(m->window, -1, SDL_RENDERER_ACCELERATED);

  FPSmanager fps;
  SDL_initFramerate(&fps);
  SDL_setFramerate(&fps, 30);

  if (!m->renderer) {
    bug("multy~ • Couldn't create renderer: %s", SDL_GetError());
    return NULL;
  }

  while (m->running) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        m->running = false;
        break;
      }
    }

    render(m->renderer, m->step);
    SDL_framerateDelay(&fps);
  }

  post("multy~ • Destroying renderer...");

  SDL_DestroyRenderer(m->renderer);

  return NULL;
}

void multy_bang(t_multy *x) {
  post("multy~ • Bang!");
  float_t a = x->step++ % 4;
  outlet_float(x->note_out, a);
}

void *multy_new() {
  t_multy *x = (t_multy *)pd_new(multy_class);

  x->note_out = outlet_new(&x->obj, &s_float);
  x->velo_out = outlet_new(&x->obj, &s_float);

  post("multy~ • Object was created");

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  x->window =
      SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

  if (!x->window) {
    bug("multy~ • Couldn't create window: %s", SDL_GetError());
    return NULL;
  }

  x->running = true;
  x->step = 0;

  // Create render thread
  pthread_create(&x->thread, NULL, run, x);

  return x;
}

void multy_free(t_multy *x) {
  x->running = false;
  pthread_join(x->thread, NULL);

  SDL_DestroyWindow(x->window);
  SDL_Quit();

  post("multy~ • Memory was freed");
}

void multy_tilde_setup() {
  multy_class = class_new(gensym("multy~"), (t_newmethod)multy_new,
                          (t_method)multy_free, sizeof(t_multy), 0, 0);

  class_addbang(multy_class, multy_bang);

  post("multy~ • External was loaded");
}
