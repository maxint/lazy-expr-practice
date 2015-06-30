#ifndef __EXPR_H__
#define __EXPR_H__

#include <cassert>

typedef unsigned index_t;

// expression
template<typename SubType>
struct Expr {
  inline const SubType& self() const {
    return *static_cast<const SubType*>(this);
  }
};

// value type for binary expression
template<typename TLhs, typename TRhs>
struct ReturnType {
  typedef typename TLhs::value_type value_type;
};

// general binary operation
template<typename OP, typename TLhs, typename TRhs>
struct BinaryMapExpr : Expr<BinaryMapExpr<OP, TLhs, TRhs> > {
  const TLhs &lhs;
  const TRhs &rhs;
  typedef typename ReturnType<TLhs, TRhs>::value_type value_type;

  inline BinaryMapExpr(const TLhs &lhs, const TRhs &rhs)
    : lhs(lhs), rhs(rhs) {}

  inline value_type Eval(index_t i, index_t j) const {
    return OP::Map(lhs.Eval(i, j), rhs.Eval(i, j));
  }
};

// template works for any binary expressions
template<typename OP, typename TLhs, typename TRhs>
inline BinaryMapExpr<OP, TLhs, TRhs>
F(const Expr<TLhs> &lhs, const Expr<TRhs> &rhs) {
  return BinaryMapExpr<OP, TLhs, TRhs>(lhs.self(), rhs.self());
}

// general unary operation
template<typename OP, typename TRValue>
struct UnaryMapExpr : Expr<UnaryMapExpr<OP, TRValue> > {
  const TRValue &rval;
  typedef typename TRValue::value_type value_type;

  inline UnaryMapExpr(const TRValue& rval) : rval(rval) {}

  inline value_type Eval(index_t i, index_t j) const {
    return OP::Map(rval.Eval(i, j));
  }
};

// template works for any unary expressions
template<typename OP, typename TRValue>
inline UnaryMapExpr<OP, TRValue>
F(const Expr<TRValue> &rval) {
  return UnaryMapExpr<OP, TRValue>(rval.self());
}

// transpose
template<typename TRValue>
struct TransposeExpr : Expr<TransposeExpr<TRValue> > {
  const TRValue &rval;
  typedef typename TRValue::value_type value_type;

  inline TransposeExpr(const TRValue& rval) : rval(rval) {}

  inline value_type Eval(index_t i, index_t j) const {
    return rval.Eval(j, i);
  }
};

template<typename EType>
inline TransposeExpr<EType> transpose(const EType& rval) {
  return TransposeExpr<EType>(rval);
}

namespace op {

struct add {
  template<typename T> inline static T Map(const T& a, const T& b) {
    return a + b;
  }
};

struct sub {
  template<typename T> inline static T Map(const T& a, const T& b) {
    return a - b;
  }
};

struct mul {
  template<typename T> inline static T Map(const T& a, const T& b) {
    return a * b;
  }
};

struct div {
  template<typename T> inline static T Map(const T& a, const T& b) {
    return a / b;
  }
};

struct maximum {
  template<typename T> inline static T Map(const T& a, const T& b) {
    return a > b ? a : b;
  }
};

struct minimum {
  template<typename T> inline static T Map(const T& a, const T& b) {
    return a < b ? a : b;
  }
};

} // namespace op

// addition
template<typename TLhs, typename TRhs>
inline BinaryMapExpr<op::add, TLhs, TRhs>
operator+(const Expr<TLhs> &lhs, const Expr<TRhs> &rhs) {
  return F<op::add>(lhs, rhs);
}

// subtraction
template<typename TLhs, typename TRhs>
inline BinaryMapExpr<op::sub, TLhs, TRhs>
operator-(const Expr<TLhs> &lhs, const Expr<TRhs> &rhs) {
  return F<op::sub>(lhs, rhs);
}

#endif /* end of include guard */

// vim: et ts=2 sts=2 sw=2
