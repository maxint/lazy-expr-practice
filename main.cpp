// Practice code for "Lazy Expression"
// https://github.com/dmlc/mshadow/tree/master/guide/exp-template

#include <cstdio>
#include <iostream>
#include "matx.h"

int main() {
  Vec3i A(1, 2, 3), B(2, 3, 4), C(3, 4, 5);
  std::cout << "A:          " << A << std::endl;
  std::cout << "B:          " << B << std::endl;
  //std::cout << "A+B+B:      " << A + B + C << std::endl;
  //std::cout << "A+max(B,C): " << A + expr::F<op::maximum>(B, C) << std::endl;

  // transpose
  printf("Transpose Test:\n");
  Matx2i D(1, 2, 3, 4), Dt, E(5, 5, 5, 5);
  Dt = D.T();
  assert(D(0,0) == Dt(0,0));
  assert(D(0,1) == Dt(1,0));
  assert(D(1,0) == Dt(0,1));
  assert(D(1,1) == Dt(1,1));
  std::cout << "D:       " << D << std::endl;
  std::cout << "E:       " << E << std::endl;
  std::cout << "D_t:     " << Dt << std::endl;

  Dt = Transpose(D + E);
  std::cout << "(D+E)_t: " << Dt << std::endl;

  Dt = (D + E).T();
  std::cout << "(D+E)_t: " << Dt << std::endl;

  return 0;
}

// vim: et ts=2 sts=2 sw=2
