#ifndef __MATX_H__
#define __MATX_H__

#include "expr.h"
#include "shape.h"
#include <ostream>

template<typename T, int m, int n=1>
struct Matx : public expr::RValueExpr<2, Matx<T, m, n>, T> {
  enum {
    kRows = m,
    kCols = n,
    kChannels = m*n,
    kIsVector = m == 1 || n == 1,
  };

  inline Matx(void) {
    for (int i = 0; i < kChannels; ++i) val_[i] = T(0);
  }

  inline explicit Matx(const T& v0) {
    SM_StaticAssert(kChannels >= 1, "");
    val_[0] = v0;
    for (int i = 1; i < kChannels; ++i) val_[i] = T(0);
  }

  inline Matx(const T& v0, const T& v1) {
    SM_StaticAssert(kChannels >= 2, "");
    val_[0] = v0;
    val_[1] = v1;
    for (int i = 2; i < kChannels; ++i) val_[i] = T(0);
  }

  inline Matx(const T& v0, const T& v1, const T& v2) {
    SM_StaticAssert(kChannels >= 3, "");
    val_[0] = v0;
    val_[1] = v1;
    val_[2] = v2;
    for (int i = 3; i < kChannels; ++i) val_[i] = T(0);
  }

  inline Matx(const T& v0, const T& v1, const T& v2, const T& v3) {
    SM_StaticAssert(kChannels >= 4, "");
    val_[0] = v0;
    val_[1] = v1;
    val_[2] = v2;
    val_[3] = v3;
    for (int i = 4; i < kChannels; ++i) val_[i] = T(0);
  }

  // construction from pointer
  inline explicit Matx(const T* ptr) {
    SM_Assert(0 != ptr);
    for (int i = 0; i < kChannels; ++i) val_[i] = ptr[i];
  }

  // conversion contructor
  template<typename T2>
  inline explicit Matx(const Matx<T2, m, n>& other) {
    for (int i = 0; i < kChannels; ++i) val_[i] = (T) other.val_[i];
  }

  // conversion operator
  template<typename T2>
  inline operator Matx<T2, m, n>(void) {
    return Matx<T2, m, n>(*this);
  }

  // construction from expression
  template<typename EType> inline Matx(const expr::Expr<EType, T>& expr) {
    this->__assign(expr);
  }

  // assignment expression
  inline Matx& operator =(const T& s) {
    return this->__assign(s);
  }
  template<typename EType> inline Matx& operator =(const expr::Expr<EType, T>& expr) {
    return this->__assign(expr);
  }

  //  accessors
  inline T& operator ()(index_t i, index_t j) {
    SM_DbgAssert(i < m && j < n);
    return val_[i*n+j];
  }
  inline const T& operator ()(index_t i, index_t j) const {
    SM_DbgAssert(i < m && j < n);
    return val_[i*n+j];
  }
  inline T& operator ()(index_t i) {
    SM_DbgAssert(i < kChannels);
    return val_[i];
  }
  inline const T& operator ()(index_t i) const {
    SM_DbgAssert(i < kChannels);
    return val_[i];
  }
  inline T& operator [](index_t i) {
    return (*this)(i);
  }
  inline const T& operator [](index_t i) const {
    return (*this)(i);
  }

  inline const T& Eval(index_t i, index_t j) const {
    return (*this)(i, j);
  }

  // creation
  inline static expr::ScalarExpr<T> All(const T& s) {
    return expr::scalar(s);
  }
  inline static expr::ScalarExpr<T> Zero(void) {
    return expr::scalar(0);
  }
  inline static expr::IdentityExpr<Matx<T, m, n> > Identity(const T& s = 1) {
    return expr::IdentityExpr<Matx<T, m, n> >(s);
  }

private:
  T val_[kChannels];
};

namespace expr {
template<typename T, int m, int n>
struct IdentityExpr<Matx<T, m, n> > : public Expr<IdentityExpr<Matx<T, m, n> >, T> {
  T scalar_;

  inline IdentityExpr<Matx<T, m, n> >(const T& s) : scalar_(s) {}

  inline T Eval(index_t i, index_t j) const {
    return (i != j) ? 0 : scalar_;
  }

  inline Shape<2> CheckShape(void) const {
    return Shape2(m, n);
  }
};
} // namespace expr

// output to stream
template<typename T, int m, int n>
inline std::ostream& operator <<(std::ostream& out, const Matx<T, m, n>& mat) {
  out << "[";
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      out << mat(i, j);
      if (j != n-1) out << ", ";
    }
    if (i != m-1) out << "; ";
  }
  out << "]";

  return out;
}

typedef Matx<float , 2, 2> Matx2f;
typedef Matx<float , 3, 3> Matx3f;
typedef Matx<float , 4, 4> Matx4f;
typedef Matx<float , 2, 1> Vec2f;
typedef Matx<float , 3, 1> Vec3f;
typedef Matx<float , 4, 1> Vec4f;

typedef Matx<double, 2, 2> Matx2d;
typedef Matx<double, 3, 3> Matx3d;
typedef Matx<double, 4, 4> Matx4d;
typedef Matx<double, 2, 1> Vec2d;
typedef Matx<double, 3, 1> Vec3d;
typedef Matx<double, 4, 1> Vec4d;

typedef Matx<int   , 2, 2> Matx2i;
typedef Matx<int   , 3, 3> Matx3i;
typedef Matx<int   , 4, 4> Matx4i;
typedef Matx<int   , 2, 1> Vec2i;
typedef Matx<int   , 3, 1> Vec3i;
typedef Matx<int   , 4, 1> Vec4i;

// vector template alias (c++11 only)
//template<typename T, int m> using Vec = Matx<T, m, 1>;

#endif /* end of include guard */

// vim: et ts=2 sts=2 sw=2
