/*
 *   __   __     __  __     __         __
 *  /\ "-.\ \   /\ \/\ \   /\ \       /\ \
 *  \ \ \-.  \  \ \ \_\ \  \ \ \____  \ \ \____
 *   \ \_\\"\_\  \ \_____\  \ \_____\  \ \_____\
 *    \/_/ \/_/   \/_____/   \/_____/   \/_____/
 *   ______     ______       __     ______     ______     ______
 *  /\  __ \   /\  == \     /\ \   /\  ___\   /\  ___\   /\__  _\
 *  \ \ \/\ \  \ \  __<    _\_\ \  \ \  __\   \ \ \____  \/_/\ \/
 *   \ \_____\  \ \_____\ /\_____\  \ \_____\  \ \_____\    \ \_\
 *    \/_____/   \/_____/ \/_____/   \/_____/   \/_____/     \/_/
 *
 * https://joshbassett.info
 *
 * Copyright (c) 2025 Joshua Bassett
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <m_pd.h>

#include "grid.h"
#include "multy.h"

#define DEFAULT_WIDTH 540
#define DEFAULT_HEIGHT 540

void step_state(state_t *state) {
  grid_t next_grid = {0};

  for (size_t y = 0; y < GRID_SIZE; y++) {
    for (size_t x = 0; x < GRID_SIZE; x++) {
      update_cell(&next_grid, &state->grid, x, y);
    }
  }

  memcpy(&state->grid, &next_grid, sizeof(grid_t));
}

static void draw_cell(SDL_Renderer *renderer, const SDL_Rect *rect, int cell) {
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

  if (cell & CELL_UP) {
    SDL_RenderDrawLines(renderer, arrows[0], 6);
  }
  if (cell & CELL_DOWN) {
    SDL_RenderDrawLines(renderer, arrows[1], 6);
  }
  if (cell & CELL_LEFT) {
    SDL_RenderDrawLines(renderer, arrows[2], 6);
  }
  if (cell & CELL_RIGHT) {
    SDL_RenderDrawLines(renderer, arrows[3], 6);
  }
}

void draw_grid(SDL_Renderer *renderer, size_t width, size_t height,
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
        draw_cell(renderer, &rect2, dir);
      }
    }
  }

  SDL_RenderPresent(renderer);
}

void multy_bang(t_multy *x) {
  post("multy~ • Bang!");
  step_state(&x->state);

  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      break;
    }
  }

  draw_grid(x->renderer, DEFAULT_WIDTH, DEFAULT_HEIGHT, &x->state.grid);
}

void *multy_new() {
  t_multy *x = (t_multy *)pd_new(multy_class);

  x->note_out = outlet_new(&x->obj, &s_float);
  x->velo_out = outlet_new(&x->obj, &s_float);

  SDL_Init(SDL_INIT_VIDEO);
  x->window =
      SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_RESIZABLE);
  if (!x->window) {
    bug("multy~ • Couldn't create window: %s", SDL_GetError());
    return NULL;
  }
  x->renderer = SDL_CreateRenderer(x->window, -1, SDL_RENDERER_ACCELERATED);
  if (!x->renderer) {
    bug("multy~ • Couldn't create renderer: %s", SDL_GetError());
  }

  post("multy~ • Object was created");

  x->running = true;
  x->state.grid.cells[0][0] = CELL_RIGHT;
  x->state.grid.cells[0][4] = CELL_DOWN;
  x->state.grid.cells[0][8] = CELL_LEFT;

  // Create render thread
  // pthread_create(&x->thread, NULL, (void *(*)(void *))run, x);

  return x;
}

void multy_free(t_multy *x) {
  x->running = false;

  // pthread_join(x->thread, NULL);

  SDL_DestroyRenderer(x->renderer);
  SDL_DestroyWindow(x->window);
  SDL_Quit();

  post("multy~ • Memory was freed");
}

void multy_setup() {
  multy_class = class_new(gensym("multy"), (t_newmethod)multy_new,
                          (t_method)multy_free, sizeof(t_multy), 0, 0);

  class_addbang(multy_class, multy_bang);

  post("multy~ • External was loaded");
}
