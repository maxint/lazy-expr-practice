#ifndef __EXPR_H__
#define __EXPR_H__

#include "base.h"
#include <cassert>

typedef unsigned index_t;

template<typename EType, typename DType> struct TransposeExpr;

// expression
template<typename SubType, typename DType>
struct Expr {
  inline const SubType& self() const {
    return *static_cast<const SubType*>(this);
  }

  TransposeExpr<SubType, DType> T() const {
    return TransposeExpr<SubType, DType>(this->self());
  }
};

//---------------------------------------------
// BinaryMapExpr
//---------------------------------------------

// general binary operation
template<typename OP, typename TLhs, typename TRhs, typename DType>
struct BinaryMapExpr : Expr<BinaryMapExpr<OP, TLhs, TRhs, DType>, DType> {
  const TLhs &lhs;
  const TRhs &rhs;

  inline explicit BinaryMapExpr(const TLhs &lhs, const TRhs &rhs)
    : lhs(lhs), rhs(rhs) {}

  inline DType Eval(index_t i, index_t j) const {
    return OP::Map(lhs.Eval(i, j), rhs.Eval(i, j));
  }
};

// template works for any binary expressions
template<typename OP, typename TLhs, typename TRhs, typename DType>
inline BinaryMapExpr<OP, TLhs, TRhs, DType>
F(const Expr<TLhs, DType> &lhs, const Expr<TRhs, DType> &rhs) {
  return BinaryMapExpr<OP, TLhs, TRhs, DType>(lhs.self(), rhs.self());
}

// plus
template<typename TLhs, typename TRhs, typename DType>
inline BinaryMapExpr<op::plus, TLhs, TRhs, DType>
operator +(const Expr<TLhs, DType> &lhs, const Expr<TRhs, DType> &rhs) {
  return F<op::plus>(lhs, rhs);
}

// minus
template<typename TLhs, typename TRhs, typename DType>
inline BinaryMapExpr<op::minus, TLhs, TRhs, DType>
operator -(const Expr<TLhs, DType> &lhs, const Expr<TRhs, DType> &rhs) {
  return F<op::minus>(lhs, rhs);
}

// multiplying
template<typename TLhs, typename TRhs, typename DType>
inline BinaryMapExpr<op::mul, TLhs, TRhs, DType>
operator *(const Expr<TLhs, DType> &lhs, const Expr<TRhs, DType> &rhs) {
  return F<op::mul>(lhs, rhs);
}

// division
template<typename TLhs, typename TRhs, typename DType>
inline BinaryMapExpr<op::div, TLhs, TRhs, DType>
operator /(const Expr<TLhs, DType> &lhs, const Expr<TRhs, DType> &rhs) {
  return F<op::div>(lhs, rhs);
}

//---------------------------------------------
// UnaryMapExpr
//---------------------------------------------

// general unary operation
template<typename OP, typename EType, typename DType>
struct UnaryMapExpr : Expr<UnaryMapExpr<OP, EType, DType>, DType> {
  const EType &rval;

  inline explicit UnaryMapExpr(const EType& rval) : rval(rval) {}

  inline DType Eval(index_t i, index_t j) const {
    return OP::Map(rval.Eval(i, j));
  }
};

// template works for any unary expressions
template<typename OP, typename EType, typename DType>
inline UnaryMapExpr<OP, EType, DType>
F(const Expr<EType, DType> &rval) {
  return UnaryMapExpr<OP, EType, DType>(rval.self());
}

// transpose
template<typename EType, typename DType>
struct TransposeExpr : Expr<TransposeExpr<EType, DType>, DType> {
  const EType &rval;

  inline TransposeExpr(const EType& rval) : rval(rval) {}

  inline DType Eval(index_t i, index_t j) const {
    return rval.Eval(j, i);
  }
};

template<typename SubType, typename DType>
inline TransposeExpr<SubType, DType> Transpose(const Expr<SubType, DType>& rval) {
  return rval.T();
}

#endif /* end of include guard */

// vim: et ts=2 sts=2 sw=2
