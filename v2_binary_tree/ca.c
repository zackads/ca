#include "ca.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static const configuration DEAD = {0, NULL, NULL};
static const configuration ALIVE = {0, NULL, NULL};

configuration *config_create(const size_t capacity) {
  configuration *c = malloc(sizeof *c);
  if (!c) {
    return NULL;
  }

  if (capacity <= 1) {
    *c = (configuration){1, &DEAD, &DEAD};
    return c;
  }

  configuration *left = config_create((capacity + 1) / 2);
  configuration *right = config_create((capacity + 1) / 2);
  if (!left || !right) {
    free(left);
    free(right);
    printf("Out of memory");
    exit(1);
  }

  *c = (configuration){left->level + 1, left, right};
  return c;
}

state config_read(const configuration *c, const cell x) {
  assert(c->level >= 1);

  const cell half = (cell)1 << (c->level - 1);
  if (x < -half || half <= x) {
    return false;
  }
  if (c->level == 1) {
    return (x < 0 ? c->left : c->right) == &ALIVE;
  }
  if (x < 0) {
    return config_read(c->left, x + half / 2);
  }
  return config_read(c->right, x - half / 2);
}

void config_set(configuration *c, const cell x, const state s) {
  assert(c->level >= 1);
  const cell half = (cell)1 << (c->level - 1);
  if (x < -half || half <= x) {
    fprintf(stderr, "%s:%d: index %ld out of bounds\n", __FILE__, __LINE__, x);
    abort();
  }
  configuration *leaf = (int)s ? &ALIVE : &DEAD;
  if (c->level == 1) {
    if (x < 0) {
      c->left = leaf;
    } else {
      c->right = leaf;
    }
  } else if (x < 0) {
    config_set(c->left, x + (half / 2), s);
  } else {
    config_set(c->right, x - (half / 2), s);
  }
}

void config_destroy(configuration *c) {
  if (c->level > 1) {
    config_destroy(c->left);
    config_destroy(c->right);
  }
  free(c);
}

state f(const rule r, const configuration *config, const cell x) {
  const state left = config_read(config, x - 1);
  const state centre = config_read(config, x);
  const state right = config_read(config, x + 1);

  return ((r >> ((4 * (long)left) + (2 * (long)centre) + (long)right)) & 1) != 0;
}

configuration *F(const rule r, const configuration *c) {
  const long half = (long)1 << (c->level - 1);
  configuration *next = config_create((size_t)half);

  for (long x = -half; x < half; x++) {
    config_set(next, x, f(r, c, x));
  }

  return next;
}
