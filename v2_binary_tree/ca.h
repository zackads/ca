#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef bool state;
typedef uint8_t rule; // A Wolfram encoding of a cellular automaton's local update rule in byte form
typedef signed long cell;

/**
 * Represent a configuration as a binary tree.
 *
 * A node at level L covers cells x ∈ [-2^(L-1), 2^(L-1)), centered on its own origin.  Its left
 * child covers the negative half and its right child the non-negative half.
 */
typedef struct configuration {
  unsigned level;
  struct configuration *left, *right;
} configuration;

configuration *config_create(size_t capacity);
state config_read(const configuration *c, cell x);
void config_set(configuration *c, cell x, state s);
void config_destroy(configuration *c);

/**
 * Apply the local update rule @param r to cell @param x in @param config and @return the result.
 */
state f(rule r, const configuration *config, cell x);

/**
 * Apply @param r globally to @param c and @return the new configuration.
 */
configuration *F(rule r, const configuration *c);
