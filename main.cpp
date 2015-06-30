// Practice code for "Lazy Expression"
// https://github.com/dmlc/mshadow/tree/master/guide/exp-template

#include <cstdio>
#include <iostream>
#include "expr_engine.h"

int main() {
  Vec3i A(1, 2, 3), B(2, 3, 4), C(3, 4, 5);
  std::cout << "A:          " << A << std::endl;
  std::cout << "B:          " << B << std::endl;
  std::cout << "C:          " << C << std::endl;
  std::cout << "A+B+C:      " << Vec3i(A + B + C) << std::endl;
  std::cout << "A+max(B,C): " << Vec3i(A + expr::F<op::maximum>(B, C)) << std::endl;

  // transpose
  printf("Transpose Test:\n");
  Matx2i D(1, 2, 3, 4), E(5, 5, 5, 5), Dt;
  Dt = D.T();
  SM_Assert(D(0,0) == Dt(0,0));
  SM_Assert(D(0,1) == Dt(1,0));
  SM_Assert(D(1,0) == Dt(0,1));
  SM_Assert(D(1,1) == Dt(1,1));
  std::cout << "D:       " << D << std::endl;
  std::cout << "E:       " << E << std::endl;
  std::cout << "D_t:     " << Dt << std::endl;
  std::cout << "D_t:     " << Matx2i(D.T()) << std::endl;

  Dt = Transpose(D + E);
  std::cout << "(D+E)_t: " << Dt << std::endl;

  Dt = (D + E).T();
  std::cout << "(D+E)_t: " << Dt << std::endl;

  return 0;
}

// vim: et ts=2 sts=2 sw=2
