#ifndef MULTY_H_
#define MULTY_H_

#include <m_pd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>

#define GRID_SIZE 8

enum cell { CELL_EMPTY, CELL_UP, CELL_DOWN, CELL_LEFT, CELL_RIGHT };

typedef enum cell cell_t;

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
