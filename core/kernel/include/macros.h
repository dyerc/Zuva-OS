#ifndef MACROS_H
#define MACROS_H

#include <types.h>

#define __expect(foo,bar) __builtin_expect((long)(foo),bar)

#define ALIGN(n,a) (((n)+(a)-1)/(a)*(a))

#define __likely(foo) __expect((foo),1)
#define __unlikely(foo) __expect((foo),0)

/* Defines an assertion for the expression that we want to happen, if if doesnt, error out. */
#ifdef DEBUG
#define ASSERT(exp) \
    if ( __unlikely( !( exp ) ) ) { \
        panic_assert(#exp, __LINE__, __FILE__); \
    }
#else
#define ASSERT (exp)
#endif /* MK_RELEASE_BUILD */

void panic_assert(const char *file, uint32_t line, const char *desc);

#endif
