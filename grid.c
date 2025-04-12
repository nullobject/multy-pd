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

#include <sys/types.h>

#include "grid.h"

void update_cell(grid_t *next_grid, const grid_t *prev_grid, size_t x,
                 size_t y) {
  if (prev_grid->cells[y][x] & CELL_UP) {
    if (y == 0) {
      next_grid->cells[y + 1][x] |= CELL_DOWN;
    } else {
      next_grid->cells[y - 1][x] |= CELL_UP;
    }
  }
  if (prev_grid->cells[y][x] & CELL_DOWN) {
    if (y == GRID_SIZE - 1) {
      next_grid->cells[y - 1][x] |= CELL_UP;
    } else {
      next_grid->cells[y + 1][x] |= CELL_DOWN;
    }
  }
  if (prev_grid->cells[y][x] & CELL_LEFT) {
    if (x == 0) {
      next_grid->cells[y][x + 1] |= CELL_RIGHT;
    } else {
      next_grid->cells[y][x - 1] |= CELL_LEFT;
    }
  }
  if (prev_grid->cells[y][x] & CELL_RIGHT) {
    if (x == GRID_SIZE - 1) {
      next_grid->cells[y][x - 1] |= CELL_LEFT;
    } else {
      next_grid->cells[y][x + 1] |= CELL_RIGHT;
    }
  }
}
