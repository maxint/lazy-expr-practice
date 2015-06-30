#ifndef __BASE_H__
#define __BASE_H__

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

#endif /* end of include guard */
