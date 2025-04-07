#ifndef MULTY_H_
#define MULTY_H_

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
  bool running;
  pthread_t thread;
  state_t state;
} t_multy;

static t_class *multy_class;

#endif
