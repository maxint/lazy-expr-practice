#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "base.h"

template <int dimension> struct Shape {
  enum {
    kDimension = dimension,
    kSubdim = dimension - 1,
  };

  inline Shape(void) {}

  inline Shape(const Shape<kDimension> &s) {
    for (int i = 0; i < kDimension; i++) {
      shape_[i] = s[i];
    }
  }

  inline index_t& operator [](index_t idx) {
    assert(idx < kDimension);
    return shape_[idx];
  }
  inline const index_t& operator [](index_t idx) const {
    assert(idx < kDimension);
    return shape_[idx];
  }

  inline bool operator ==(const Shape<kDimension> &s) const {
    for (int i = 0; i < kDimension; i++) {
      if (s[i] != this->shape_[i])
        return false;
    }
    return true;
  }
  inline bool operator !=(const Shape<kDimension> &s) const {
    return !(*this == s);
  }

  // flatten the higher dimension to second dimension, return a 2D shape
  inline Shape<2> FlatTo2D(void) const {
    Shape<2> s;
    s[1] = shape_[kSubdim];
    index_t ymax = 1;
    for (int i = 0; i < kSubdim; ++i) {
      ymax *= shape_[i];
    }
    s[0] = ymax;
    return s;
  }

  // return number of valid elements
  inline size_t Size(void) const {
    size_t size = shape_[0];
    for (int i = 1; i < kDimension; ++i) {
      size *= shape_[i];
    }
    return size;
  }

  // get subshape that takes off largest dimension
  inline Shape<kSubdim> SubShape(void) const {
    Shape<kSubdim> s;
    for (int i = 0; i < kSubdim; ++i) {
      s[i] = shape_[i + 1];
    }
    return s;
  }

  // slice the shape from start to end
  template<int dimstart, int dimend>
  inline Shape<dimend - dimstart> Slice(void) const {
    Shape<dimend - dimstart> s;
    for (int i = dimstart; i < dimend; ++i) {
      s[i - dimstart] = shape_[i];
    }
    return s;
  }

private:
  index_t shape_[kDimension];
};

//------------------------------------------------
// useful construction functions to generate shape
//-------------------------------------------------

inline Shape<1> Shape1(index_t s0) {
  Shape<1> s; s[0] = s0;
  return s;
}
inline Shape<2> Shape2(index_t s0, index_t s1) {
  Shape<2> s; s[0] = s0; s[1] = s1;
  return s;
}
inline Shape<3> Shape3(index_t s0, index_t s1, index_t s2) {
  Shape<3> s;
  s[0] = s0; s[1] = s1; s[2] = s2;
  return s;
}
inline Shape<4> Shape4(index_t s0, index_t s1, index_t s2, index_t s3) {
  Shape<4> s;
  s[0] = s0; s[1] = s1; s[2] = s2; s[3] = s3;
  return s;
}

#endif /* end of include guard */
