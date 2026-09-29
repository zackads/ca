#include <stdbool.h>
#include <stddef.h>
typedef struct ht ht;

/*
 * With thanks to https://benhoyt.com/writings/hash-table-in-c/
 */

// Create a hash table and return a pointer to it, or NULL if out of memory.
ht *ht_create(void);

// Free memory allocated for hash table, including allocated keys.
void ht_destroy(ht *table);

// Get
void *ht_get(ht *table, const char *key);

// Set
const char *ht_set(ht *table, const char *key, void *value);

// Delete
// TODO

// Number of items in the hash table
size_t ht_length(ht *table);

typedef struct {
  const char *key; // current key
  void *value;     // current value

  ht *_table;    // pointer to hash table being iterated
  size_t _index; // current index into ht._entries
} hti;

// Construct new hash table iterator
hti ht_iterator(ht *table);

// Move iterator to next item in hash table and return true, returning false if there are no more
// items.
bool ht_next(hti *it);
