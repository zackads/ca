#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ca.h"

static void usage(const char *prog) {
  fprintf(stderr, "usage: %s <rule> <steps> [seed [placement]]\n", prog);
  fprintf(stderr, "  rule       even Wolfram rule number, 0-254\n");
  fprintf(stderr, "  steps      number of generations to run\n");
  fprintf(stderr, "  seed       initial cells, e.g. 1101 (default 1)\n");
  fprintf(stderr,
          "  placement  position of the seed's leftmost 1 relative to the origin (default 0)\n");
}

// Parse an integer in [min, max] into *n. Return false if s is not one.
static bool parse_long(const char *s, long min, long max, long *n) {
  char *end;
  errno = 0;
  *n = strtol(s, &end, 10);
  return errno == 0 && *s != '\0' && *end == '\0' && *n >= min && *n <= max;
}

// Print cells lo..hi of a one-dimensional configuration.
static void print_row(const configuration c, const int lo, const int hi) {
  for (int x = lo; x <= hi; x++) {
    printf("%s", (int)config_read(c, x) ? "█" : "·");
  }
  printf("\n");
}

int main(const int argc, char **argv) {
  if (argc < 3 || argc > 5) {
    usage(argv[0]);
    return 1;
  }

  long rule_arg, steps, placement = 0;
  const char *seed = argc >= 4 ? argv[3] : "1";
  if (!parse_long(argv[1], 0, 255, &rule_arg) || !parse_long(argv[2], 0, 100000, &steps) ||
      (argc == 5 && !parse_long(argv[4], -100000, 100000, &placement))) {
    usage(argv[0]);
    return 1;
  }

  const rule r = (uint8_t)rule_arg;
  if (r % 2 != 0) {
    fprintf(stderr, "rule %d maps 000 to 1, so 0 is not quiescent; only even rules are supported\n",
            r);
    return 1;
  }

  if (strspn(seed, "01") != strlen(seed)) {
    fprintf(stderr, "seed must contain only 0 and 1\n");
    return 1;
  }

  // Put the leftmost live cell of the seed at the placement
  const int offset = (int)strspn(seed, "0") - (int)placement;
  const int left = (int)placement; // The leftmost live cell
  int right = left;                // The rightmost live cell
  const char *last = strrchr(seed, '1');
  if (last != NULL) {
    right = (int)(last - seed) - offset;
  }

  // The pattern spreads by at most one cell each way per step. Pad both sides equally so the
  // origin is central.
  int hi = (right > 0 ? right : 0) + (int)steps;
  int lo = (left < 0 ? left : 0) - (int)steps;
  if (-lo > hi) {
    hi = -lo;
  }
  lo = -hi;

  // Every live cell stays within [lo, hi], so a margin beyond it keeps the finite edges from
  // affecting the result.
  configuration c = config_create((size_t)hi + 2);
  for (int i = 0; seed[i] != '\0'; i++) {
    if (seed[i] == '1') {
      config_set(c, i - offset, true);
    }
  }

  printf("%*s↓\n", -lo, ""); // Mark the origin
  print_row(c, lo, hi);
  for (long t = 0; t < steps; t++) {
    const configuration next = F(r, c);

    config_destroy(c);
    c = next;
    print_row(c, lo, hi);
  }

  config_destroy(c);
  return 0;
}
