#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <m_pd.h>

#include "multy.h"

#define DEFAULT_WIDTH 600
#define DEFAULT_HEIGHT 600

void step_state(state_t *state) {
  grid_t next_grid = {0};

  for (size_t y = 0; y < GRID_SIZE; y++) {
    for (size_t x = 0; x < GRID_SIZE; x++) {
      switch (state->grid.cells[y][x]) {
      case CELL_UP:
        if (y == 0 || state->grid.cells[y - 1][x]) {
          next_grid.cells[y][x] = CELL_DOWN;
        } else {
          next_grid.cells[y - 1][x] = CELL_UP;
        }
        break;
      case CELL_DOWN:
        if (y == GRID_SIZE - 1 || state->grid.cells[y + 1][x]) {
          next_grid.cells[y][x] = CELL_UP;
        } else {
          next_grid.cells[y + 1][x] = CELL_DOWN;
        }
        break;
      case CELL_LEFT:
        if (x == 0 || state->grid.cells[y][x - 1]) {
          next_grid.cells[y][x] = CELL_RIGHT;
        } else {
          next_grid.cells[y][x - 1] = CELL_LEFT;
        }
        break;
      case CELL_RIGHT:
        if (x == GRID_SIZE - 1 || state->grid.cells[y][x + 1]) {
          next_grid.cells[y][x] = CELL_LEFT;
        } else {
          next_grid.cells[y][x + 1] = CELL_RIGHT;
        }
        break;
      default:
        // do nothing
        break;
      }
    }
  }

  memcpy(&state->grid, &next_grid, sizeof(grid_t));
}

static void draw_arrow(SDL_Renderer *renderer, int dir, const SDL_Rect *rect) {
  int a = rect->w / 6;
  int b = rect->w / 3;
  SDL_Point arrows[4][6] = {
      {
          {rect->x + (rect->w / 2), rect->y + (rect->h - a)},
          {rect->x + (rect->w / 2), rect->y + b},
          {rect->x + (rect->w / 2) - a, rect->y + b},
          {rect->x + (rect->w / 2), rect->y + a},
          {rect->x + (rect->w / 2) + a, rect->y + b},
          {rect->x + (rect->w / 2) - a, rect->y + b},
      },
      {
          {rect->x + (rect->w / 2), rect->y + a},
          {rect->x + (rect->w / 2), rect->y + (rect->h - b)},
          {rect->x + (rect->w / 2) - a, rect->y + (rect->h - b)},
          {rect->x + (rect->w / 2), rect->y + (rect->h - a)},
          {rect->x + (rect->w / 2) + a, rect->y + (rect->h - b)},
          {rect->x + (rect->w / 2) - a, rect->y + (rect->h - b)},
      },
      {
          {rect->x + (rect->w - a), rect->y + (rect->h / 2)},
          {rect->x + b, rect->y + (rect->h / 2)},
          {rect->x + b, rect->y + (rect->h / 2) - a},
          {rect->x + a, rect->y + (rect->h / 2)},
          {rect->x + b, rect->y + (rect->h / 2) + a},
          {rect->x + b, rect->y + (rect->h / 2) - a},
      },
      {
          {rect->x + a, rect->y + (rect->h / 2)},
          {rect->x + (rect->w - b), rect->y + (rect->h / 2)},
          {rect->x + (rect->w - b), rect->y + (rect->h / 2) - a},
          {rect->x + (rect->w - a), rect->y + (rect->h / 2)},
          {rect->x + (rect->w - b), rect->y + (rect->h / 2) + a},
          {rect->x + (rect->w - b), rect->y + (rect->h / 2) - a},
      },
  };

  SDL_RenderDrawLines(renderer, arrows[dir - 1], 6);
}

void render(SDL_Renderer *renderer, size_t width, size_t height,
            const grid_t *grid) {
  size_t size = width > height ? height : width;
  size_t cell_width = size / GRID_SIZE;
  size_t cell_height = size / GRID_SIZE;

  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderClear(renderer);

  for (size_t y = 0; y < GRID_SIZE; y++) {
    for (size_t x = 0; x < GRID_SIZE; x++) {
      cell_t dir = grid->cells[y][x];
      SDL_Point origin = {x * cell_width, y * cell_height};
      SDL_Rect rect1 = {origin.x, origin.y, cell_width, cell_height};
      SDL_Rect rect2 = {origin.x + 1, origin.y + 1, cell_width - 2,
                        cell_height - 2};

      SDL_SetRenderDrawColor(renderer, 0xcc, 0xcc, 0xcc, 0xff);
      SDL_RenderDrawRect(renderer, &rect1);

      if (dir) {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xa3, 0x75, 0xff);
        SDL_RenderFillRect(renderer, &rect2);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
        draw_arrow(renderer, dir, &rect2);
      }
    }
  }

  SDL_RenderPresent(renderer);
}

void *run(t_multy *x) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  FPSmanager fps;
  size_t width = DEFAULT_WIDTH;
  size_t height = DEFAULT_HEIGHT;

  post("multy~ • Starting renderer...");

  if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window,
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
      } else if (e.type == SDL_WINDOWEVENT &&
                 e.window.event == SDL_WINDOWEVENT_RESIZED) {
        width = e.window.data1;
        height = e.window.data2;
      }
    }

    render(renderer, width, height, &x->state.grid);

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
  x->state.grid.cells[0][0] = CELL_RIGHT;
  x->state.grid.cells[0][4] = CELL_DOWN;
  x->state.grid.cells[0][7] = CELL_LEFT;

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
