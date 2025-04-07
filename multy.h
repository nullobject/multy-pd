#ifndef MULTY_H_
#define MULTY_H_

#include <SDL2/SDL.h>
#include <m_pd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>

typedef struct {
  t_object obj;
  t_outlet *note_out, *velo_out;
  pthread_t thread;
  bool running;
  uint step;
  SDL_Window *window;
  SDL_Renderer *renderer;
} t_multy;

static t_class *multy_class;

void multy_dsp(t_multy *x, t_signal **sp, short *count);
t_int *multy_perform(t_int *w);

#endif
