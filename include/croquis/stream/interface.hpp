// Copyright 2021-2022 Lawrence Livermore National Security, LLC and other
// croquis Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: MIT

#ifndef _CROQUIS_STREAM_INTERFACE_HPP
#define _CROQUIS_STREAM_INTERFACE_HPP

#include <croquis/hash/hash.hpp>

#include <croquis/transform/CountSketch.hpp>
#include <croquis/transform/FWHT.hpp>

#include <croquis/sketch/Dense.hpp>
#include <croquis/sketch/Promotable.hpp>
#include <croquis/sketch/Sparse.hpp>

#include <croquis/sketch/Sketch.hpp>

#include <croquis/stream/Multi.hpp>
#include <croquis/stream/Summary.hpp>

#if __has_include(<ygm/comm.hpp>)
#include <croquis/stream/Distributed.hpp>
// #include <ygm/detail/ygm_ptr.hpp>
#endif

#include <map>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// Multi Stream Type Presets
////////////////////////////////////////////////////////////////////////////////

namespace croquis {
namespace stream {

template <template <typename, typename...> class SketchFunc,
          template <typename, typename> class ContainerType,
          template <typename> class MergeOp, typename KeyType, typename RegType,
          typename... Args>
using MultiLocal =
    Multi<CountingSummary, croquis::sketch::Sketch, SketchFunc, ContainerType,
          MergeOp, KeyType, RegType, std::shared_ptr, Args...>;

template <template <typename, typename> class ContainerType, typename KeyType,
          typename RegType>
using MultiLocalCountSketch =
    MultiLocal<croquis::transform::CountSketchFunctor, ContainerType, std::plus,
               KeyType, RegType, croquis::hash::MulAddShift>;

template <typename KeyType, typename RegType>
using MultiLocalFWHT =
    MultiLocal<croquis::transform::FWHTFunctor, croquis::sketch::Dense,
               std::plus, KeyType, RegType>;

}  // namespace stream
}  // namespace croquis

#if __has_include(<ygm/comm.hpp>)
// namespace croquis {
// namespace stream {

// // high level map types

// template <template <typename, typename...> class SketchFunc,
//           template <typename, typename> class ContainerType,
//           template <typename> class MergeOp, typename KeyType, typename
//           RegType, typename... Args>
// using MultiCommunicable =
//     Multi<CountingSummary, croquis::sketch::Sketch, SketchFunc,
//     ContainerType,
//           MergeOp, KeyType, RegType, ygm::ygm_ptr, Args...>;

// template <template <typename, typename> class ContainerType, typename
// KeyType,
//           typename RegType>
// using MultiCommunicableCountSketch =
//     MultiCommunicable<croquis::transform::CountSketchFunctor, ContainerType,
//                       std::plus, KeyType, RegType,
//                       croquis::hash::MulAddShift>;

// template <typename KeyType, typename RegType>
// using MultiCommunicableFWHT =
//     MultiCommunicable<croquis::transform::FWHTFunctor,
//     croquis::sketch::Dense,
//                       std::plus, KeyType, RegType>;

// }  // namespace stream
// }  // namespace croquis

namespace croquis {
namespace stream {

template <template <typename, typename...> class SketchFunc,
          template <typename, typename> class ContainerType,
          template <typename> class MergeOp, typename KeyType, typename RegType,
          typename... Args>
using CountingDistributed =
    Distributed<CountingSummary, croquis::sketch::Sketch, SketchFunc,
                ContainerType, MergeOp, KeyType, RegType, Args...>;

template <template <typename, typename> class ContainerType, typename KeyType,
          typename RegType>
using CountingDistributedCountSketch =
    CountingDistributed<croquis::transform::CountSketchFunctor, ContainerType,
                        std::plus, KeyType, RegType,
                        croquis::hash::MulAddShift>;

template <typename KeyType, typename RegType>
using CountingDistributedFWHT =
    CountingDistributed<croquis::transform::FWHTFunctor, croquis::sketch::Dense,
                        std::plus, KeyType, RegType>;

}  // namespace stream
}  // namespace croquis

#endif
#endif