#include <stddef.h>
#include <stdint.h>

typedef uint8_t state_t;

typedef state_t (*local_update_rule)(const state_t *neighbourhood, size_t n, void *ctx);

typedef struct {
  const int *const * neighbours;
  size_t len;
} neighbourhood;

typedef struct {
  size_t            d; // The dimension of the cellular space
  size_t            s; // S = { 0, 1, ..., s - 1 }
  neighbourhood     N;
  local_update_rule f;
} ca;

typedef struct {
  const size_t      *shape; // Extent in each dimension
  size_t            len;
  state_t           *cells;
} configuration;

void step(const ca *r, const configuration *in, configuration *out) {
  // 1.

  // 2.
}