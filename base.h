#ifndef __BASE_H__
#define __BASE_H__

#include <cstdlib> // size_t
#include "log.h"

typedef unsigned index_t;

// operators
namespace op {

struct plus {
  template<typename T> inline static T Map(const T& a, const T& b) {
    return a + b;
  }
};

struct minus {
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

// savers
namespace sv {

struct saveto {
  template<typename T> inline static void Save(T& a, const T& b) {
    a = b;
  }
};

struct plusto {
  template<typename T> inline static void Save(T& a, const T& b) {
    a += b;
  }
};

struct minusto {
  template<typename T> inline static void Save(T& a, const T& b) {
    a -= b;
  }
};

struct multo {
  template<typename T> inline static void Save(T& a, const T& b) {
    a *= b;
  }
};

struct divto {
  template<typename T> inline static void Save(T& a, const T& b) {
    a /= b;
  }
};

} // namespace sv

#endif /* end of include guard */

// vim: et ts=2 sts=2 sw=2
