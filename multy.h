#ifndef MULTY_H_
#define MULTY_H_

#include <m_pd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>

#define GRID_SIZE 8

#define CELL_EMPTY 0
#define CELL_UP 1
#define CELL_DOWN 2
#define CELL_LEFT 3
#define CELL_RIGHT 4

typedef int cell_t;

typedef struct {
  cell_t cells[GRID_SIZE][GRID_SIZE];
} grid_t;

typedef struct {
  grid_t grid;
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
