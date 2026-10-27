#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef bool state;
typedef uint8_t rule; // A Wolfram encoding of a cellular automaton's local update rule in byte form
typedef signed long cell;

/**
 * Represent a configuration as one array for the positive indices (including 0) and another for the
 * negative indices.
 */
typedef struct {
  size_t capacity;
  state *left;
  state *right;
} configuration;

configuration config_create(size_t capacity);
state config_read(configuration c, cell x);
void config_set(configuration c, cell x, state s);
void config_destroy(configuration c);

/**
 * Apply the local update rule @param r to cell @param x in @param config and @return the result.
 */
state f(rule r, configuration config, cell x);

/**
 * Apply @param r globally to @param config and @return the new configuration.
 */
configuration F(rule r, configuration config);
