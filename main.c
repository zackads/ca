#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ht.h"

const uint8_t DEFAULT_WIDTH = 80;
const uint8_t SEED_SIZE = 4;
const uint8_t T_MAX = 255;

int term_width(void) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0)
        return ws.ws_col;

    const char *cols = getenv("COLUMNS");
    if (cols && atoi(cols) > 0)
        return atoi(cols);

    return DEFAULT_WIDTH;
}

void vis() {
    printf("width = %d\n", term_width());
    const uint8_t WIDTH = term_width();

    bool seed[SEED_SIZE] = { 1, 1, 0, 1 };
    bool old[WIDTH];
    bool new[WIDTH];

    // Initialise to zero
    for (uint8_t i = 0; i < WIDTH; i++) {
        old[i] = 0;
    }

    // Place the seed at the origin
    uint8_t origin_i = (int)((WIDTH - SEED_SIZE) / 2);
    for (int i = 0; i < SEED_SIZE; i++) {
        old[origin_i + i] = seed[i];
    }

    // Print the initial configuration
    for (uint8_t i = 0; i < WIDTH; i++) {
        if (old[i]) { printf("█"); } else { printf("·"); }
    }
    printf("\n");

    while (true) {
        // When the user hits enter, continue
        getchar();

        // Apply the update rule
        for (uint8_t i = 0; i < WIDTH; i++) {
            if (i == 0) {
                new[i] = 0 ^ old[1];
            } else if (i == WIDTH - 1) {
                new[i] = old[WIDTH - 1] ^ 0;
            } else {
                new[i] = old[i - 1] ^ old[i + 1];
            }
        }

        // Print the new configuration
        for (uint8_t i = 0; i < WIDTH - 1; i++) {
            if (new[i]) { printf("█"); } else { printf("·"); }
        }
        // printf("\n");

        // new -> old
        for (uint8_t i = 0; i < WIDTH; i++) {
            old[i] = new[i];
        }
    }
}

void exit_nomem(void) {
    fprintf(stderr, "out of memory\n");
    exit(1);
}

int main(void) {
    printf("Let's play around with our new hash table!\n");
    // Let's play around with our new hash table.
    ht* table = ht_create();
    if (table == NULL) {
        exit_nomem();
    }

    // Read next word from stdin (up to max 100)
    char word[101];
    while (scanf("%100s", word) != EOF) {
        // Look up word.
        void* value = ht_get(table, word);
        if (value != NULL) {
            // Already exists, increment int that value points to
            int* pcount = (int*)value;
            (*pcount)++;
            continue;
        }

        // Word not found, allocate space for new int and set to 1.
        int* pcount = malloc(sizeof(int));
        if (pcount == NULL) {
            exit_nomem();
        }
        *pcount = 1;
        if (ht_set(table, word, pcount) == NULL) {
            exit_nomem();
        }
    }

    // Print out words and frequencies, freeing values as we go.
    hti it = ht_iterator(table);
    while (ht_next(&it)) {
        printf("%s %d\n", it.key, *(int*)it.value);
        free(it.value);
    }

    // Show the number of unique words
    printf("%d\n", (int)ht_length(table));

    ht_destroy(table);
    return 0;
}

