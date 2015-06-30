#ifndef __EXPR_H__
#define __EXPR_H__

#include "base.h"

namespace expr {

template<typename EType, typename DType> struct TransposeExpr;
template<typename DstDType, typename SrcDType, typename EType> struct TypeCastExpr;
template<typename Saver, int dim, typename RValue, typename DType> struct ExprEngine;

//---------------------------------------------
// Expr
//---------------------------------------------

template<typename SubType, typename DType>
struct Expr {
  inline const SubType& self(void) const {
    return *static_cast<const SubType*>(this);
  }

  inline SubType& self(void) {
    return *static_cast<SubType*>(this);
  }

  inline TransposeExpr<SubType, DType> T(void) const {
    return TransposeExpr<SubType, DType>(this->self());
  }

  template<typename DstDType>
  inline TypeCastExpr<DstDType, DType, SubType> Cast(void) const {
    return TypeCastExpr<DstDType, DType, SubType>(this->self());
  }
};

//---------------------------------------------
// ScalarExpr
//---------------------------------------------

template<typename DType>
struct ScalarExpr : public Expr<ScalarExpr<DType>, DType> {
  DType scalar_;

  inline ScalarExpr(const DType& s) : scalar_(s) {}

  inline const DType& Eval(index_t, index_t) const {
    return scalar_;
  }
};

template<typename DType> inline ScalarExpr<DType> scalar(const DType& s) {
  return ScalarExpr<DType>(s);
}


//---------------------------------------------
// TypeCastExpr
//---------------------------------------------

template<typename DstDType, typename SrcDType, typename EType>
struct TypeCastExpr : public Expr<TypeCastExpr<DstDType, SrcDType, EType>, DstDType> {
  const EType& expr_;

  inline explicit TypeCastExpr(const EType& e) : expr_(e) {}

  inline DstDType Eval(index_t i, index_t j) const {
    return static_cast<DstDType>(expr_.Eval(i, j));
  }
};

template<typename DstDType, typename SrcDType, typename EType>
TypeCastExpr<DstDType, SrcDType, EType> Cast(const Expr<EType, SrcDType>& expr) {
  return TypeCastExpr<DstDType, SrcDType, EType>(expr.self());
}

//---------------------------------------------
// RValueExpr
//---------------------------------------------

template<int dim, typename Container, typename DType>
struct RValueExpr : public Expr<Container, DType> {
  // inplace operatiors with scalar
  template<typename EType> inline Container& operator +=(const DType& s) {
    ExprEngine<sv::plusto, dim, Container, DType>::Eval(this->self(), scalar(s));
    return this->self();
  }

  template<typename EType> inline Container& operator -=(const DType& s) {
    ExprEngine<sv::minusto, dim, Container, DType>::Eval(this->self(), scalar(s));
    return this->self();
  }

  template<typename EType> inline Container& operator *=(const DType& s) {
    ExprEngine<sv::multo, dim, Container, DType>::Eval(this->self(), scalar(s));
    return this->self();
  }

  template<typename EType> inline Container& operator /=(const DType& s) {
    ExprEngine<sv::divto, dim, Container, DType>::Eval(this->self(), scalar(s));
    return this->self();
  }

  // assignment (MUST be overrided by derived classes)
  template<typename EType> inline Container& __assign(const DType& s) {
    ExprEngine<sv::saveto, dim, Container, DType>::Eval(this->self(), scalar(s));
    return this->self();
  }

  template<typename EType> inline Container& __assign(const Expr<EType, DType>& expr) {
    ExprEngine<sv::saveto, dim, Container, DType>::Eval(this->self(), expr.self());
    return this->self();
  }

  // inplace operatiors with expression
  template<typename EType> inline Container& operator +=(const Expr<EType, DType>& expr) {
    ExprEngine<sv::plusto, dim, Container, DType>::Eval(this->self(), expr.self());
    return this->self();
  }

  template<typename EType> inline Container& operator -=(const Expr<EType, DType>& expr) {
    ExprEngine<sv::minusto, dim, Container, DType>::Eval(this->self(), expr.self());
    return this->self();
  }

  template<typename EType> inline Container& operator *=(const Expr<EType, DType>& expr) {
    ExprEngine<sv::multo, dim, Container, DType>::Eval(this->self(), expr.self());
    return this->self();
  }

  template<typename EType> inline Container& operator /=(const Expr<EType, DType>& expr) {
    ExprEngine<sv::divto, dim, Container, DType>::Eval(this->self(), expr.self());
    return this->self();
  }
};

//---------------------------------------------
// BinaryMapExpr
//---------------------------------------------

// general binary operation
template<typename OP, typename TLhs, typename TRhs, typename DType>
struct BinaryMapExpr : Expr<BinaryMapExpr<OP, TLhs, TRhs, DType>, DType> {
  const TLhs &lhs_;
  const TRhs &rhs_;

  inline explicit BinaryMapExpr(const TLhs &lhs, const TRhs &rhs)
    : lhs_(lhs), rhs_(rhs) {}

  inline DType Eval(index_t i, index_t j) const {
    return OP::Map(lhs_.Eval(i, j), rhs_.Eval(i, j));
  }
};

// template works for any binary expressions
template<typename OP, typename TLhs, typename TRhs, typename DType>
inline BinaryMapExpr<OP, TLhs, TRhs, DType>
F(const Expr<TLhs, DType> &lhs, const Expr<TRhs, DType> &rhs) {
  return BinaryMapExpr<OP, TLhs, TRhs, DType>(lhs.self(), rhs.self());
}

//---------------------------------------------
// Binary Operations Override
//---------------------------------------------

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
  const EType &expr_;

  inline explicit UnaryMapExpr(const EType& expr) : expr_(expr) {}

  inline DType Eval(index_t i, index_t j) const {
    return OP::Map(expr_.Eval(i, j));
  }
};

// template works for any unary expressions
template<typename OP, typename EType, typename DType>
inline UnaryMapExpr<OP, EType, DType>
F(const Expr<EType, DType> &expr) {
  return UnaryMapExpr<OP, EType, DType>(expr.self());
}

//---------------------------------------------
// TransposeExpr
//---------------------------------------------

template<typename EType, typename DType>
struct TransposeExpr : Expr<TransposeExpr<EType, DType>, DType> {
  const EType &expr_;

  inline TransposeExpr(const EType& expr) : expr_(expr) {}

  inline DType Eval(index_t i, index_t j) const {
    return expr_.Eval(j, i);
  }

  inline const EType& T(void) const {
    return expr_;
  }
};

template<typename SubType, typename DType>
inline TransposeExpr<SubType, DType> Transpose(const Expr<SubType, DType>& rval) {
  return rval.T();
}

} // namespace expr

#endif /* end of include guard */

// vim: et ts=2 sts=2 sw=2
