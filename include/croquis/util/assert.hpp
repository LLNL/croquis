// Copyright 2021-2022 Lawrence Livermore National Security, LLC and other
// croquis Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: MIT

#ifndef _CROQUIS_UTIL_ASSERT_HPP
#define _CROQUIS_UTIL_ASSERT_HPP

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>

// work  on this:  https://github.com/lattera/glibc/blob/master/assert/assert.c
void croquis_release_assert_fail(const char *assertion, const char *file,
                                 unsigned int line, const char *function) {
  std::stringstream ss;
  ss << " " << assertion << " " << file << ":" << line << " " << function
     << std::endl;
  throw std::runtime_error(ss.str());
}

#define CROQUIS_ASSERT_DEBUG(expr) assert(expr)

#define CROQUIS_ASSERT_RELEASE(expr) \
  (static_cast<bool>(expr)           \
       ? void(0)                     \
       : croquis_release_assert_fail(#expr, __FILE__, __LINE__, ""))

#endif