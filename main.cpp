// Practice code for "Lazy Expression"
// https://github.com/dmlc/mshadow/tree/master/guide/exp-template

#include <cstdio>
#include "matx.h"

int main() {
  const int n = 3;
  Matx<int, n> A(1, 2, 3), B(2, 3, 4), C(3, 4, 5);
  // run expression, this expression is longer:)
  A = B + C + C;
  for (int i = 0; i < n; ++i) {
    printf("#%d: %d == %d + %d + %d\n", i,
           A[i], B[i], C[i], C[i]);
  }
  A = B + C + F<op::maximum>(B, C);
  for (int i = 0; i < n; ++i) {
    printf("#%d: %d == %d + %d + max(%d, %d)\n", i,
           A[i], B[i], C[i], B[i], C[i]);
  }

  // transpose
  printf("Transpose Test:\n");
  Matx<int, 2, 2> D(1, 2, 3, 4), Dt, E(5, 5, 5, 5);
  Dt = D.t();
  assert(D(0,0) == Dt(0,0));
  assert(D(0,1) == Dt(1,0));
  assert(D(1,0) == Dt(0,1));
  assert(D(1,1) == Dt(1,1));
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      printf("# %d,%d: %d -> %d\n", i, j, D(i, j), Dt(i, j));
    }
  }
  Dt = transpose(D + E);
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      printf("# %d,%d: (%d + %d) -> %d\n", i, j, D(i, j), E(i, j), Dt(i, j));
    }
  }
  return 0;
}

// vim: et ts=2 sts=2 sw=2
