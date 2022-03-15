// Copyright 2021-2022 Lawrence Livermore National Security, LLC and other
// croquis Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: MIT

#ifndef _KROWKEE_SKETCH_INTERFACE_HPP
#define _KROWKEE_SKETCH_INTERFACE_HPP

#include <krowkee/hash/hash.hpp>

#include <krowkee/transform/CountSketch.hpp>
#include <krowkee/transform/FWHT.hpp>

#include <krowkee/sketch/Dense.hpp>
#include <krowkee/sketch/Promotable.hpp>
#include <krowkee/sketch/Sparse.hpp>

#include <krowkee/sketch/Sketch.hpp>

#if __has_include(<ygm/comm.hpp>)
#include <ygm/detail/ygm_ptr.hpp>
#endif

#if __has_include(<boost/container/flat_map.hpp>)
#include <boost/container/flat_map.hpp>
#endif

#include <map>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// Sketch Type Presets
////////////////////////////////////////////////////////////////////////////////

namespace croquis {
namespace sketch {

template <template <typename, typename...> class SketchFunc,
          template <typename, typename> class ContainerType,
          template <typename> class MergeOp, typename RegType, typename... Args>
using LocalSketch = Sketch<SketchFunc, ContainerType, MergeOp, RegType,
                           std::shared_ptr, Args...>;

template <template <typename, typename> class ContainerType, typename RegType>
using LocalCountSketch =
    LocalSketch<croquis::transform::CountSketchFunctor, ContainerType,
                std::plus, RegType, croquis::hash::MulAddShift>;

template <typename RegType>
using LocalFWHT = LocalSketch<croquis::transform::FWHTFunctor,
                              croquis::sketch::Dense, std::plus, RegType>;

}  // namespace sketch
}  // namespace croquis

#if __has_include(<ygm/comm.hpp>)
namespace croquis {
namespace sketch {

template <template <typename, typename...> class SketchFunc,
          template <typename, typename> class ContainerType,
          template <typename> class MergeOp, typename RegType, typename... Args>
using CommunicableSketch =
    Sketch<SketchFunc, ContainerType, MergeOp, RegType, ygm::ygm_ptr, Args...>;

template <template <typename, typename> class ContainerType, typename RegType>
using CommunicableCountSketch =
    CommunicableSketch<croquis::transform::CountSketchFunctor, ContainerType,
                       std::plus, RegType, croquis::hash::MulAddShift>;

template <typename RegType>
using CommunicableFWHT =
    CommunicableSketch<croquis::transform::FWHTFunctor, croquis::sketch::Dense,
                       std::plus, RegType>;

}  // namespace sketch
}  // namespace croquis
#endif

////////////////////////////////////////////////////////////////////////////////
// Sparse Container Presets
////////////////////////////////////////////////////////////////////////////////

namespace croquis {
namespace sketch {

template <typename RegType, typename MergeOp, typename KeyType>
using MapSparse = Sparse<RegType, MergeOp, std::map, KeyType>;

template <typename RegType, typename MergeOp>
using MapSparse32 = MapSparse<RegType, MergeOp, std::uint32_t>;

#if __has_include(<boost/container/flat_map.hpp>)
template <typename RegType, typename MergeOp, typename KeyType>
using FlatMapSparse =
    Sparse<RegType, MergeOp, boost::container::flat_map, KeyType>;

template <typename RegType, typename MergeOp>
using FlatMapSparse32 = FlatMapSparse<RegType, MergeOp, std::uint32_t>;
#endif

}  // namespace sketch
}  // namespace croquis

////////////////////////////////////////////////////////////////////////////////
// Promotable Container Presets
////////////////////////////////////////////////////////////////////////////////

namespace croquis {
namespace sketch {

template <typename RegType, typename MergeOp, typename KeyType>
using MapPromotable = Promotable<RegType, MergeOp, std::map, KeyType>;

template <typename RegType, typename MergeOp>
using MapPromotable32 = MapPromotable<RegType, MergeOp, std::uint32_t>;

#if __has_include(<boost/container/flat_map.hpp>)
template <typename RegType, typename MergeOp, typename KeyType>
using FlatMapPromotable =
    Promotable<RegType, MergeOp, boost::container::flat_map, KeyType>;

template <typename RegType, typename MergeOp>
using FlatMapPromotable32 = FlatMapPromotable<RegType, MergeOp, std::uint32_t>;
#endif

}  // namespace sketch
}  // namespace croquis

#endif