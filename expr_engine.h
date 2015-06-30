#ifndef __EXPR_ENGINE_H__
#define __EXPR_ENGINE_H__

#include "expr.h"
#include "shape.h"

#include <utility>

namespace expr {

template<typename Saver, typename Container, typename DType, typename EType>
inline void MapExpr(RValueExpr<Container, DType>& dst, const Expr<EType, DType>& expr);

template<typename Saver, typename Container, typename DType>
struct ExprEngine {
  template<typename EType> inline static void Eval(RValueExpr<Container, DType>& dst, const Expr<EType, DType>& expr) {
    MapExpr<Saver>(dst, expr);
  }
};

//---------------------------------------------
// ShapeCheck
//---------------------------------------------
template<int dim, typename EType>
struct ShapeCheck {
  inline static Shape<dim> Check(const EType& expr);
};
template<int dim, typename DType>
struct ShapeCheck<dim, ScalarExpr<DType> > {
  inline static Shape<dim> Check(const ScalarExpr<DType>& expr) {
    Shape<dim> s; s[0] = 0;
    return s;
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
    return s1;
  }
};

} // namespace expr

#endif /* end of include guard */

// vim: et ts=2 sts=2 sw=2
