#pragma once

#ifdef JADE_ENABLE_LIBASSERT
#include <libassert/assert.hpp>
#else
#include <cassert>
#endif

#ifdef JADE_ENABLE_LIBASSERT
#define JADE_ASSERT(...) ASSERT(__VA_ARGS__)
#else
#define JADE_ASSERT(X, ...) assert(X)
#endif

