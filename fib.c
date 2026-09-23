//
// Created by Zack Adlington on 22/09/2026.
//
#include <stdio.h>

const int N_MAX = 10;
int memo[N_MAX];

// fib(n) = fib(n - 1) + fib(n - 2)
int fib_rec(const int n) {
  if (n <= 2) return 1;
  return fib_rec(n - 1) + fib_rec(n - 2);
}

int fib_memo(const int n) {
  if (n <= 2) return 1;
  if (memo[n - 1] && memo[n - 2]) return memo[n - 1] + memo[n - 2];

  memo[n - 1] = fib_memo(n - 1);
  memo[n - 2] = fib_memo(n - 2);
  return fib_memo(n);
}

int main(void) {
  for (int i = 0; i <= N_MAX; i++) {
    printf("[%d]", memo[i]);
  }
  printf("\n");

  for (int i = 1; i <= 10; i++) {
    printf("fib(%d) = %d\n", i, fib_memo(i));
  }

  for (int i = 0; i <= N_MAX; i++) {
    printf("[%d]", memo[i]);
  }
  printf("\n");
}