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

  BinaryMapExpr(const TLhs &lhs, const TRhs &rhs)
    : lhs(lhs), rhs(rhs) {}

  inline value_type Eval(index_t i, index_t j) const {
    return OP::Map(lhs.Eval(i, j), rhs.Eval(i, j));
  }
};

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

// template works for any expressions
template<typename OP, typename TLhs, typename TRhs>
inline BinaryMapExpr<OP, TLhs, TRhs> F(const Expr<TLhs> &lhs, const Expr<TRhs> &rhs) {
  return BinaryMapExpr<OP, TLhs, TRhs>(lhs.self(), rhs.self());
}

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
