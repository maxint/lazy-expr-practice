#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <cstdarg>
#include <cstdio>
#include <cstdlib> // exit()

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

#define SM_Error_(tag, fmt, ...)    fprintf(stderr, tag ": " fmt "\n", ##__VA_ARGS__)
#define SM_Error(fmt, ...)          SM_Error_("Error", fmt, ##__VA_ARGS__)
#define SM_Assert(expr, fmt, ...)   if ((expr)) ; else SM_Error_("Assert", fmt, ##__VA_ARGS)
#ifdef _DEBUG
# define SM_DbgAssert(expr, fmt, ...)  SM_Assert(expr, fmt, ##__VA_ARGS__)
#else
# define SM_DbgAssert(expr, fmt, ...)
#endif // _DEBUG
#define SM_StaticAssert(expr, msg)  static_assert((expr), msg)

#endif /* end of include guard */
