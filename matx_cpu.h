#ifndef __MATX_CPU_H__
#define __MATX_CPU_H__

#include "matx.h"

namespace expr {

template<typename Saver, typename DType, int m, int n, typename EType>
inline void MapExpr(RValueExpr<Matx<DType, m, n>, DType>& dst, const Expr<EType, DType>& expr) {
  const EType &src = expr.self();
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      Saver::Save(dst.self()(i, j), src.Eval(i, j));
    }
  }
}

} // namespace expr

#endif /* end of include guard */

// vim: et ts=2 sts=2 sw=2
