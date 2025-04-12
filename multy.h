#ifndef MULTY_H_
#define MULTY_H_

#include <m_pd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>

#define GRID_SIZE 9

// clang-format off
#define CELL_EMPTY 0x0
#define CELL_UP    0x1
#define CELL_DOWN  0x2
#define CELL_LEFT  0x4
#define CELL_RIGHT 0x8
// clang-format on

// Lookup table for the number of bits set in a nibble
static const uint8_t NIBBLE_LOOKUP[8] = {
  0, 1, 1, 2, 1, 2, 2, 3,
};

typedef uint cell_t;

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
