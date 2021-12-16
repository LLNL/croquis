// Copyright 2021-2022 Lawrence Livermore National Security, LLC and other
// croquis Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: MIT

#ifndef _CROQUIS_UTIL_CHECK_HPP
#define _CROQUIS_UTIL_CHECK_HPP

#include <croquis/util/assert.hpp>

inline void CHECK_CONDITION(const bool success, const std::string &msg) {
  std::cout << ((success == true) ? "passed" : "failed") << " " << msg
            << " test" << std::endl;
  CROQUIS_ASSERT_RELEASE(success);
}

template <typename ExceptType, typename FuncType, typename... Args>
inline void CHECK_THROWS(const FuncType &func, const std::string &msg,
                         Args &...args) {
  bool        caught = false;
  std::string str;
  try {
    func(args...);
  } catch (ExceptType &e) {
    str    = e.what();
    caught = true;
  }
  if (caught == true) {
    std::cout << "caught expected " << msg << " exception:\n\t" << str
              << std::endl;
  } else {
    std::cout << "failed to catch expected " << msg << " exception"
              << std::endl;
  }
  CROQUIS_ASSERT_RELEASE(caught);
}

template <typename ExceptType, typename FuncType, typename... Args>
inline void CHECK_DOES_NOT_THROW(const FuncType &func, const std::string &msg,
                                 const Args &...args) {
  try {
    func(args...);
  } catch (ExceptType &e) {
    std::cout << msg << " incorrectly threw exception \"" << e.what() << "\""
              << std::endl;
    CROQUIS_ASSERT_RELEASE(false);
  }
}

#endif