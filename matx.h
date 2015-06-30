#ifndef __MATX_H__
#define __MATX_H__

#include "expr.h"

template<typename T, int m, int n=1>
struct Matx : public Expr<Matx<T, m, n> > {
  typedef T value_type;

  enum {
    rows = m,
    cols = n,
    channels = m*n,
    is_vector = m == 1 || n == 1,
  };

  inline Matx() {
    for (int i = 0; i < channels; ++i) val[i] = T(0);
  }

  inline Matx(const T& v0) {
    static_assert(channels >= 1, "");
    val[0] = v0;
    for (int i = 1; i < channels; ++i) val[i] = T(0);
  }

  inline Matx(const T& v0, const T& v1) {
    static_assert(channels >= 2, "");
    val[0] = v0;
    val[1] = v1;
    for (int i = 2; i < channels; ++i) val[i] = T(0);
  }

  inline Matx(const T& v0, const T& v1, const T& v2) {
    static_assert(channels >= 3, "");
    val[0] = v0;
    val[1] = v1;
    val[2] = v2;
    for (int i = 3; i < channels; ++i) val[i] = T(0);
  }

  inline Matx(const T& v0, const T& v1, const T& v2, const T& v3) {
    static_assert(channels >= 4, "");
    val[0] = v0;
    val[1] = v1;
    val[2] = v2;
    val[3] = v3;
    for (int i = 4; i < channels; ++i) val[i] = T(0);
  }

  // construction from pointer
  inline explicit Matx(const T* ptr) {
    assert(0 != ptr);
    for (int i = 0; i < channels; ++i) val[i] = ptr[i];
  }

  // conversion contructor
  template<typename T2>
  inline explicit Matx(const Matx<T2, m, n>& other) {
    for (int i = 0; i < channels; ++i) val[i] = (T) other.val[i];
  }

  // conversion operator
  template<typename T2>
  inline operator Matx<T2, m, n>() {
    return Matx<T2, m, n>(*this);
  }

  // assignment expression
  template<typename EType> inline Matx &operator=(const Expr<EType> &src_) {
    const EType &src = src_.self();
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        (*this)(i, j) = src.Eval(i, j);
      }
    }
    return *this;
  }

  //  accessors
  inline const T& operator ()(index_t i, index_t j) const {
    assert(i < m && j < n);
    return val[i*n+j];
  }
  inline T& operator ()(index_t i, index_t j) {
    assert(i < m && j < n);
    return val[i*n+j];
  }
  inline const T& operator ()(index_t i) const {
    assert(i < channels);
    return val[i];
  }
  inline T& operator ()(index_t i) {
    assert(i < channels);
    return val[i];
  }
  inline const T& operator [](index_t i) const {
    return (*this)(i);
  }
  inline T& operator [](index_t i) {
    return (*this)(i);
  }

  inline const T& Eval(index_t i, index_t j) const {
    return (*this)(i, j);
  }

  // transpose
  inline TransposeExpr<Matx> t() const {
    return transpose(*this);
  }

  T val[channels];
};

typedef Matx<float , 2, 2> Matx2f;
typedef Matx<float , 3, 3> Matx3f;
typedef Matx<float , 4, 4> Matx4f;
typedef Matx<double, 2, 2> Matx2d;
typedef Matx<double, 3, 3> Matx3d;
typedef Matx<double, 4, 4> Matx4d;

typedef Matx<float , 2, 1> Vec2f;
typedef Matx<float , 3, 1> Vec3f;
typedef Matx<float , 4, 1> Vec4f;
typedef Matx<double, 2, 1> Vec2d;
typedef Matx<double, 3, 1> Vec3d;
typedef Matx<double, 4, 1> Vec4d;

// vector template alias (c++11 only)
//template<typename T, int m> using Vec = Matx<T, m, 1>;

#endif /* end of include guard */

// vim: et ts=2 sts=2 sw=2
