#ifndef STDINT_H
#define STDINT_H

#if defined(_MSC_VER) && (_MSC_VER < 1600)

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#else

#include <stdint.h>

#endif

#endif