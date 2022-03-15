// Copyright 2021-2022 Lawrence Livermore National Security, LLC and other
// krowkee Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: MIT

#include <krowkee/sketch/interface.hpp>

#include <krowkee/util/ygm_tests.hpp>

using Dense32CountSketch =
    krowkee::sketch::CommunicableCountSketch<krowkee::sketch::Dense,
                                             std::int32_t>;

using MapSparse32CountSketch =
    krowkee::sketch::CommunicableCountSketch<krowkee::sketch::MapSparse32,
                                             std::int32_t>;

using MapPromotable32CountSketch =
    krowkee::sketch::CommunicableCountSketch<krowkee::sketch::MapPromotable32,
                                             std::int32_t>;

#if __has_include(<boost/container/flat_map.hpp>)
using FlatMapSparse32CountSketch =
    krowkee::sketch::CommunicableCountSketch<krowkee::sketch::FlatMapSparse32,
                                             std::int32_t>;

using FlatMapPromotable32CountSketch = krowkee::sketch::CommunicableCountSketch<
    krowkee::sketch::FlatMapPromotable32, std::int32_t>;
#endif

using Dense32FWHT = krowkee::sketch::CommunicableFWHT<std::int32_t>;

template <typename T>
using make_ptr_functor_t = make_ygm_ptr_functor_t<T>;