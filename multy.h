#ifndef MULTY_H_
#define MULTY_H_

#define WIDTH 800
#define HEIGHT 600

#include <SDL2/SDL.h>
#include <m_pd.h>

typedef struct {
  t_object obj;
  t_float x_f;
  SDL_Window *window;
  SDL_Renderer *renderer;
} t_multy;

static t_class *multy_class;

void multy_dsp(t_multy *x, t_signal **sp, short *count);
t_int *multy_perform(t_int *w);

#endif
