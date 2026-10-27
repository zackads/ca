#include "ca.h"

#include <stdio.h>
#include <stdlib.h>

configuration config_create(size_t capacity) {
  bool *left = calloc(capacity, sizeof(bool));
  bool *right = calloc(capacity, sizeof(bool));

  if (left == NULL || right == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(1);
  }

  const configuration config = {capacity, left, right};

  return config;
}

state config_read(const configuration c, const cell x) {
  if (x < -(long)c.capacity || (long)c.capacity <= x) {
    return false;
  }
  if (x >= 0) {
    return c.right[x];
  }
  return c.left[(size_t)((cell)c.capacity + x)];
}

void config_set(const configuration c, const cell x, const state s) {
  if (x < -(long)c.capacity || (long)c.capacity <= x) {
    fprintf(stderr, "%s:%d: index %ld out of bounds for capacity %zu\n", __FILE__, __LINE__, x,
            c.capacity);
    abort();
  }
  if (x >= 0) {
    c.right[x] = s;
  } else {
    c.left[(size_t)((cell)c.capacity + x)] = s;
  }
}

void config_destroy(const configuration c) {
  free(c.left);
  free(c.right);
}

state f(const rule r, const configuration config, const cell x) {
  const state left = config_read(config, x - 1);
  const state centre = config_read(config, x);
  const state right = config_read(config, x + 1);

  return ((r >> ((4 * (long)left) + (2 * (long)centre) + (long)right)) & 1) != 0;
}

configuration F(const rule r, const configuration config) {
  const configuration next = config_create(config.capacity);

  // Negative cells
  for (long x = -(long)config.capacity; x < 0; x++) {
    config_set(next, x, f(r, config, x));
  }

  // Positive cells
  for (long x = 0; x < (long)config.capacity; x++) {
    config_set(next, x, f(r, config, x));
  }

  return next;
}
