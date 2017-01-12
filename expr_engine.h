#ifndef __EXPR_ENGINE_H__
#define __EXPR_ENGINE_H__

#include "expr.h"
#include "matx.h"
#include "shape.h"

#include <utility>

namespace expr {

//---------------------------------------------------
// Runtime Shape Checking
// Note: shape is dependent on rvalue, for example:
//    Vec3f v = scalar(0);
//---------------------------------------------------

template<int dim, typename EType>
struct ShapeCheck {
  inline static Shape<dim> Check(const EType& expr);
};
template<typename DType, int m, int n>
struct ShapeCheck<2, Matx<DType, m, n> > {
  inline static Shape<2> Check(const Matx<DType, m, n>&) {
    return Shape2(m, n);
  }
};
template<int dim, typename DType>
struct ShapeCheck<dim, ScalarExpr<DType> > {
  inline static Shape<dim> Check(const ScalarExpr<DType>& expr) {
    Shape<dim> s; s[0] = 0;
    return s;
  }
};
template<int dim, typename Container>
struct ShapeCheck<dim, IdentityExpr<Container> > {
  inline static Shape<dim> Check(const IdentityExpr<Container>& expr) {
    return expr.CheckShape();
  }
};
template<int dim, typename DstDType, typename SrcDType, typename EType>
struct ShapeCheck<dim, TypeCastExpr<DstDType, SrcDType, EType> > {
  inline static Shape<dim> Check(const TypeCastExpr<DstDType, SrcDType, EType>& expr) {
    return ShapeCheck<dim, EType>::Check(expr.expr_);
  }
};
template<int dim, typename EType, typename DType>
struct ShapeCheck<dim, TransposeExpr<EType, DType> > {
  inline static Shape<dim> Check(const TransposeExpr<EType, DType>& expr) {
    Shape<dim> s = ShapeCheck<dim, EType>::Check(expr.expr_);
    std::swap(s[0], s[1]);
    return s;
  }
};
template<int dim, typename OP, typename TLhs, typename TRhs, typename DType>
struct ShapeCheck<dim, BinaryMapExpr<OP, TLhs, TRhs, DType> > {
  inline static Shape<dim> Check(const BinaryMapExpr<OP, TLhs, TRhs, DType>& expr) {
    Shape<dim> s1 = ShapeCheck<dim, TLhs>::Check(expr.lhs_);
    Shape<dim> s2 = ShapeCheck<dim, TRhs>::Check(expr.rhs_);
    if (s1[0] == 0) return s2;
    if (s2[0] == 0) return s1;
    SM_Assert_(s1 == s2, "BinaryMapExp: Shapes of operands are not the same");
    return s1;
  }
};
template<int dim, typename OP, typename EType, typename DType>
struct ShapeCheck<dim, UnaryMapExpr<OP, EType, DType> > {
  inline static Shape<dim> Check(const UnaryMapExpr<OP, EType, DType>& expr) {
    Shape<dim> s = ShapeCheck<dim, EType>::Check(expr.expr_);
    return s;
  }
};

//---------------------------------------------------
// MapExpr
//---------------------------------------------------

template<typename Saver, int dim, typename Container, typename DType, typename EType>
inline void MapExpr(RValueExpr<dim, Container, DType>& dexpr, const Expr<EType, DType>& expr) {
  const EType &src = expr.self();
  Container &dst = dexpr.self();
  Shape<dim> eshape = ShapeCheck<dim, EType>::Check(src);
  Shape<dim> dshape = ShapeCheck<dim, Container>::Check(dst);
  SM_Assert_(eshape[0] == 0 || eshape == dshape,
             "Assignment: Shape of expression are not consistent with target");
  for (index_t i = 0; i < dshape[0]; ++i) {
    for (index_t j = 0; j < dshape[1]; ++j) {
      Saver::Save(dst(i, j), src.Eval(i, j));
    }
  }
}

template<typename Saver, int dim, typename Container, typename DType>
struct ExprEngine {
  template<typename EType> inline static void Eval(RValueExpr<dim, Container, DType>& dst, const Expr<EType, DType>& expr) {
    MapExpr<Saver, dim>(dst, expr);
  }
};

} // namespace expr

#endif /* end of include guard */

// vim: et ts=2 sts=2 sw=2
