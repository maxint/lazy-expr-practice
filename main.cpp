// Practice code for "Lazy Expression"
// https://github.com/dmlc/mshadow/tree/master/guide/exp-template

#include <cstdio>
#include <iostream>
#include "expr_engine.h"

#define ENTER_TEST()  printf("- Running %s\n", __func__)

void test_assignment() {
  ENTER_TEST();

  Vec3i A = expr::scalar(1);
  SM_Assert(A[0] == 1 && A[1] == 1 && A[2] == 1);
}

void test_simple_expr() {
  ENTER_TEST();

  Vec3i A(1, 2, 3), B(2, 3, 4), C(3, 4, 5);
  Vec3i D = A + B + C;
  Vec3i E = A + expr::F<op::maximum>(B, C);
  for (int i = 0; i < 3; ++i) {
    SM_Assert(D[i] == A[i] + B[i] + C[i]);
    SM_Assert(E[i] == A[i] + std::max(B[i], C[i]));
  }
}

void test_transpose() {
  ENTER_TEST();

  Matx2i D(1, 2, 3, 4), E(5, 5, 5, 5);
  Matx2i Dt = D.T();

  SM_Assert(D(0,0) == Dt(0,0));
  SM_Assert(D(0,1) == Dt(1,0));
  SM_Assert(D(1,0) == Dt(0,1));
  SM_Assert(D(1,1) == Dt(1,1));

  Matx2i Dt1 = Transpose(D + E);
  SM_Assert(D(0,0)+E(0,0) == Dt1(0,0));
  SM_Assert(D(0,1)+E(0,1) == Dt1(1,0));
  SM_Assert(D(1,0)+E(1,0) == Dt1(0,1));
  SM_Assert(D(1,1)+E(1,1) == Dt1(1,1));

  Matx2i Dt2 = (D + E).T();
  SM_Assert(D(0,0)+E(0,0) == Dt2(0,0));
  SM_Assert(D(0,1)+E(0,1) == Dt2(1,0));
  SM_Assert(D(1,0)+E(1,0) == Dt2(0,1));
  SM_Assert(D(1,1)+E(1,1) == Dt2(1,1));
}

void test_matx_init() {
  ENTER_TEST();

  Matx2i A = Matx2i::All(2);
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      SM_Assert(A(i, j) == 2);
    }
  }
  Matx2i B = Matx2i::Zero();
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      SM_Assert(B(i, j) == 0);
    }
  }
  Matx2i C = Matx2i::Identity(2);
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      SM_Assert(C(i, j) == (i == j ? 2 : 0));
    }
  }
}

int main() {
  printf("Start testing\n");

  test_assignment();
  test_simple_expr();
  test_transpose();
  test_matx_init();

  printf("End of testing\n");

  return 0;
}

// vim: et ts=2 sts=2 sw=2
