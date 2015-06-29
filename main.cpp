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
  return 0;
}

// vim: et ts=2 sts=2 sw=2
