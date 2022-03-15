// Copyright 2021-2022 Lawrence Livermore National Security, LLC and other
// croquis Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: MIT

#ifndef _KROWKEE_STREAM_INTERFACE_HPP
#define _KROWKEE_STREAM_INTERFACE_HPP

#include <krowkee/hash/hash.hpp>

#include <krowkee/transform/CountSketch.hpp>
#include <krowkee/transform/FWHT.hpp>

#include <krowkee/sketch/Dense.hpp>
#include <krowkee/sketch/Promotable.hpp>
#include <krowkee/sketch/Sparse.hpp>

#include <krowkee/sketch/Sketch.hpp>

#include <krowkee/stream/Multi.hpp>
#include <krowkee/stream/Summary.hpp>

#if __has_include(<ygm/comm.hpp>)
#include <krowkee/stream/Distributed.hpp>
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