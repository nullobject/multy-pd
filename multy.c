#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_render.h>
#include <m_pd.h>

#include "multy.h"

#define WIDTH 600
#define HEIGHT 600

void step_state(state_t *state) {
  // TODO:: Update state
  state->step++;
}

void render(SDL_Renderer *renderer, state_t *state) {
  SDL_Rect rect = {state->step * 50 % WIDTH, 0, 50, 50};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 255);
  SDL_RenderFillRect(renderer, &rect);
  SDL_RenderPresent(renderer);
}

void *run(t_multy *x) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  FPSmanager fps;

  post("multy~ • Starting renderer...");

  if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &window,
                                  &renderer)) {
    bug("multy~ • Couldn't create window: %s", SDL_GetError());
    return NULL;
  }

  SDL_initFramerate(&fps);
  SDL_setFramerate(&fps, 30);

  while (x->running) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        x->running = false;
        break;
      }
    }

    render(renderer, &x->state);

    SDL_framerateDelay(&fps);
  }

  post("multy~ • Stopping renderer...");

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  return NULL;
}

void multy_bang(t_multy *x) {
  post("multy~ • Bang!");
  step_state(&x->state);
}

void *multy_new() {
  t_multy *x = (t_multy *)pd_new(multy_class);

  x->note_out = outlet_new(&x->obj, &s_float);
  x->velo_out = outlet_new(&x->obj, &s_float);

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  post("multy~ • Object was created");

  x->running = true;

  // Create render thread
  pthread_create(&x->thread, NULL, (void *(*)(void *))run, x);

  return x;
}

void multy_free(t_multy *x) {
  x->running = false;

  pthread_join(x->thread, NULL);

  SDL_Quit();

  post("multy~ • Memory was freed");
}

void multy_tilde_setup() {
  multy_class = class_new(gensym("multy~"), (t_newmethod)multy_new,
                          (t_method)multy_free, sizeof(t_multy), 0, 0);

  class_addbang(multy_class, multy_bang);

  post("multy~ • External was loaded");
}
