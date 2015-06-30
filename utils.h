#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <cstdarg>

namespace utils {

/*! \brief error message buffer length */
const int kPrintBuffer = 1 << 12;

inline std::string Format(bool exp, const char *fmt, ...) {
  std::string msg(kPrintBuffer, '\0');
  va_list args;
  va_start(args, fmt);
  vsnprintf(&msg[0], kPrintBuffer, fmt, args);
  va_end(args);
  return msg;
}

}  // namespace utils

#endif /* end of include guard */
