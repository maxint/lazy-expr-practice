#ifndef __LOG_H__
#define __LOG_H__

#include <cstdio>
#include <cstdlib> // exit()

#define SM_Error(fmt, ...)  fprintf(stderr, "Error: " fmt " in %s(), %s:%d\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__), exit(-1)
#define SM_Assert(expr)     if ((expr)) ; else SM_Error("Assert failed")
#define SM_Assert_(expr, fmt, ...) if ((expr)) ; else SM_Error(fmt, ##__VA_ARGS__)
#ifdef _DEBUG
# define SM_DbgAssert(expr) SM_Assert(expr)
#else
# define SM_DbgAssert(expr)
#endif // _DEBUG
#define SM_StaticAssert(expr, msg)  static_assert((expr), msg)

#endif /* end of include guard */
