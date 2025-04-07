#ifndef MULTY_H_
#define MULTY_H_

#include <SDL2/SDL.h>
#include <m_pd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>

typedef struct {
  uint step;
} state_t;

typedef struct {
  t_object obj;
  t_outlet *note_out, *velo_out;
  pthread_t thread;
  bool running;
  state_t state;
  SDL_Window *window;
  SDL_Renderer *renderer;
} t_multy;

static t_class *multy_class;

#endif
