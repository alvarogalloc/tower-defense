module;
#include <exception>
#include <algorithm>
#include <fstream>
#include <memory>
#include <source_location>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

export module stdutils;

export namespace std {
    using std::exception;
using std::span;
#ifndef _LIBCPP_HAS_NO_LOCALIZATION
using std::basic_filebuf;

#ifndef _LIBCPP_HAS_NO_FILESYSTEM
using std::swap;
#endif

using std::filebuf;
#ifndef _LIBCPP_HAS_NO_WIDE_CHARACTERS
using std::wfilebuf;
#endif

using std::basic_ifstream;

using std::ifstream;
#ifndef _LIBCPP_HAS_NO_WIDE_CHARACTERS
using std::wifstream;
#endif

using std::basic_ofstream;

using std::ofstream;
#ifndef _LIBCPP_HAS_NO_WIDE_CHARACTERS
using std::wofstream;
#endif

using std::basic_fstream;

using std::fstream;
#ifndef _LIBCPP_HAS_NO_WIDE_CHARACTERS
using std::wfstream;
#endif
#endif// _LIBCPP_HAS_NO_LOCALIZATION
}// namespace std
export namespace std {
using std::runtime_error;
using std::pair;
using std::source_location;
using std::pointer_traits;
using std::to_address;
using std::align;
using std::vector;
using std::size_t;
using std::uint32_t;
using std::assume_aligned;
using std::allocator_arg;
using std::allocator_arg_t;
using std::uses_allocator;
using std::uses_allocator_v;
using std::uses_allocator_construction_args;
using std::make_obj_using_allocator;
using std::uninitialized_construct_using_allocator;
using std::allocator_traits;

using std::allocator;
using std::operator==;
using std::addressof;
using std::uninitialized_default_construct;
using std::uninitialized_default_construct_n;
namespace ranges {
  using std::ranges::uninitialized_default_construct;
  using std::ranges::uninitialized_default_construct_n;
}// namespace ranges
using std::uninitialized_value_construct;
using std::uninitialized_value_construct_n;

namespace ranges {
  using std::ranges::uninitialized_value_construct;
  using std::ranges::uninitialized_value_construct_n;
}// namespace ranges

using std::uninitialized_copy;
using std::uninitialized_copy_n;

namespace ranges {
  using std::ranges::uninitialized_copy;
  using std::ranges::uninitialized_copy_result;

  using std::ranges::uninitialized_copy_n;
  using std::ranges::uninitialized_copy_n_result;
}// namespace ranges

using std::uninitialized_move;
using std::uninitialized_move_n;

namespace ranges {
  using std::ranges::uninitialized_move;
  using std::ranges::uninitialized_move_result;

  using std::ranges::uninitialized_move_n;
  using std::ranges::uninitialized_move_n_result;
}// namespace ranges

using std::uninitialized_fill;
using std::uninitialized_fill_n;

namespace ranges {
  using std::ranges::uninitialized_fill;
  using std::ranges::uninitialized_fill_n;
}// namespace ranges

// [specialized.construct], construct_at
using std::construct_at;

namespace ranges {
  using std::ranges::construct_at;
}
// [specialized.destroy], destroy
using std::destroy;
using std::destroy_at;
using std::destroy_n;

namespace ranges {
  using std::ranges::destroy;
  using std::ranges::destroy_at;
  using std::ranges::destroy_n;
}// namespace ranges

// [unique.ptr], class template unique_ptr
using std::default_delete;
using std::unique_ptr;

using std::make_unique;
using std::make_unique_for_overwrite;

using std::operator<;
using std::operator>;
using std::operator<=;
using std::operator>=;
using std::operator<=>;

using std::operator<<;

// [util.smartptr.weak.bad], class bad_weak_ptr
using std::bad_weak_ptr;

// [util.smartptr.shared], class template shared_ptr
using std::shared_ptr;

// [util.smartptr.shared.create], shared_ptr creation
using std::allocate_shared;
using std::allocate_shared_for_overwrite;
using std::make_shared;
using std::make_shared_for_overwrite;

// [util.smartptr.shared.spec], shared_ptr specialized algorithms
using std::swap;

// [util.smartptr.shared.cast], shared_ptr casts
using std::const_pointer_cast;
using std::dynamic_pointer_cast;
using std::reinterpret_pointer_cast;
using std::static_pointer_cast;
using std::weak_ptr;

// [util.smartptr.weak.spec], weak_ptr specialized algorithms

// [util.smartptr.ownerless], class template owner_less
using std::owner_less;

// [util.smartptr.enab], class template enable_shared_from_this
using std::enable_shared_from_this;

// [util.smartptr.hash], hash support
using std::hash;
}// namespace std


export namespace std {
// [char.traits], character traits
using std::char_traits;

// [basic.string], basic_string
using std::basic_string;
using std::basic_string_view;
using std::string_view;

using std::operator+;
using std::operator==;
using std::operator<=>;

// [string.special], swap
using std::swap;

// [string.io], inserters and extractors
using std::operator>>;
using std::operator<<;
using std::getline;

// [string.erasure], erasure
using std::erase;
using std::erase_if;

// basic_string typedef-names
using std::string;
using std::u16string;
using std::u32string;
#ifndef _LIBCPP_HAS_NO_CHAR8_T
using std::u8string;
#endif
#ifndef _LIBCPP_HAS_NO_WIDE_CHARACTERS
using std::wstring;
#endif

// [string.conversions], numeric conversions
using std::stod;
using std::stof;
using std::stoi;
using std::stol;
using std::stold;
using std::stoll;
using std::stoul;
using std::stoull;
using std::to_string;
#ifndef _LIBCPP_HAS_NO_WIDE_CHARACTERS
using std::to_wstring;
#endif

namespace pmr {
  using std::pmr::basic_string;
  using std::pmr::string;
  using std::pmr::u16string;
  using std::pmr::u32string;
#ifndef _LIBCPP_HAS_NO_CHAR8_T
  using std::pmr::u8string;
#endif
#ifndef _LIBCPP_HAS_NO_WIDE_CHARACTERS
  using std::pmr::wstring;
#endif
}// namespace pmr

// [basic.string.hash], hash support
using std::hash;

inline namespace literals {
  inline namespace string_literals {
    // [basic.string.literals], suffix for basic_string literals
    using std::literals::string_literals::operator""s;
  }// namespace string_literals
}// namespace literals
}// namespace std
export namespace std {
// [unord.map], class template unordered_­map
using std::unordered_map;

// [unord.multimap], class template unordered_­multimap
using std::unordered_multimap;

using std::operator==;

using std::swap;

// [unord.map.erasure], erasure for unordered_­map
using std::erase_if;

namespace pmr {
  using std::pmr::unordered_map;
  using std::pmr::unordered_multimap;
}// namespace pmr
}// namespace std


// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

export namespace std {
namespace ranges {
  // [algorithms.results], algorithm result types
  using std::ranges::in_found_result;
  using std::ranges::in_fun_result;
  using std::ranges::in_in_out_result;
  using std::ranges::in_in_result;
  using std::ranges::in_out_out_result;
  using std::ranges::in_out_result;
#if _LIBCPP_STD_VER >= 23
  using std::ranges::in_value_result;
#endif
  using std::ranges::min_max_result;
  // using std::ranges::out_value_result;
}// namespace ranges

// [alg.nonmodifying], non-modifying sequence operations
// [alg.all.of], all of
using std::all_of;
namespace ranges {
  using std::ranges::all_of;
}

// [alg.any.of], any of
using std::any_of;
namespace ranges {
  using std::ranges::any_of;
}

// [alg.none.of], none of
using std::none_of;
namespace ranges {
  using std::ranges::none_of;
}

#if _LIBCPP_STD_VER >= 23
// [alg.contains], contains
namespace ranges {
  using std::ranges::contains;
}// namespace ranges
#endif// _LIBCPP_STD_VER >= 23

// [alg.foreach], for each
using std::for_each;

namespace ranges {
  using std::ranges::for_each;
  using std::ranges::for_each_result;
}// namespace ranges

using std::for_each_n;

namespace ranges {
  using std::ranges::for_each_n_result;

  using std::ranges::for_each_n;
}// namespace ranges

// [alg.find], find
using std::find;
using std::find_if;
using std::find_if_not;

namespace ranges {
  using std::ranges::find;
  using std::ranges::find_if;
  using std::ranges::find_if_not;
}// namespace ranges

namespace ranges {
#if 0
    using std::ranges::find_last;
    using std::ranges::find_last_if;
    using std::ranges::find_last_if_not;
#endif
}// namespace ranges

// [alg.find.end], find end
using std::find_end;

namespace ranges {
  using std::ranges::find_end;
}

// [alg.find.first.of], find first
using std::find_first_of;

namespace ranges {
  using std::ranges::find_first_of;
}

// [alg.adjacent.find], adjacent find
using std::adjacent_find;

namespace ranges {
  using std::ranges::adjacent_find;
}

// [alg.count], count
using std::count;
using std::count_if;

namespace ranges {
  using std::ranges::count;
  using std::ranges::count_if;
}// namespace ranges

// [mismatch], mismatch
using std::mismatch;

namespace ranges {
  using std::ranges::mismatch_result;

  using std::ranges::mismatch;
}// namespace ranges

// [alg.equal], equal
using std::equal;

namespace ranges {
  using std::ranges::equal;
}

// [alg.is.permutation], is permutation
using std::is_permutation;

namespace ranges {
  using std::ranges::is_permutation;
}

// [alg.search], search
using std::search;

namespace ranges {
  using std::ranges::search;
}

using std::search_n;

namespace ranges {
  using std::ranges::search_n;
}

namespace ranges {
#if _LIBCPP_STD_VER >= 23
  // [alg.starts.with], starts with
  using std::ranges::starts_with;

  // [alg.ends.with], ends with
  using std::ranges::ends_with;

  // [alg.fold], fold
  using std::ranges::fold_left;
  using std::ranges::fold_left_with_iter;
  using std::ranges::fold_left_with_iter_result;
#if 0
    using std::ranges::fold_left_first;
    using std::ranges::fold_right;
    using std::ranges::fold_right_last;
    using std::ranges::fold_left_with_iter;
    using std::ranges::fold_left_first_with_iter;
    using std::ranges::fold_left_first_with_iter;
#endif
#endif// _LIBCPP_STD_VER >= 23
}// namespace ranges

// [alg.modifying.operations], mutating sequence operations
// [alg.copy], copy
using std::copy;

namespace ranges {
  using std::ranges::copy;
  using std::ranges::copy_result;
}// namespace ranges

using std::copy_n;

namespace ranges {
  using std::ranges::copy_n;
  using std::ranges::copy_n_result;
}// namespace ranges

using std::copy_if;

namespace ranges {
  using std::ranges::copy_if;
  using std::ranges::copy_if_result;
}// namespace ranges

using std::copy_backward;

namespace ranges {
  using std::ranges::copy_backward;
  using std::ranges::copy_backward_result;
}// namespace ranges

// [alg.move], move
using std::move;

namespace ranges {
  using std::ranges::move;
  using std::ranges::move_result;
}// namespace ranges

using std::move_backward;

namespace ranges {
  using std::ranges::move_backward;
  using std::ranges::move_backward_result;
}// namespace ranges

// [alg.swap], swap
using std::swap_ranges;

namespace ranges {
  using std::ranges::swap_ranges;
  using std::ranges::swap_ranges_result;
}// namespace ranges

using std::iter_swap;

// [alg.transform], transform
using std::transform;

namespace ranges {
  using std::ranges::binary_transform_result;
  using std::ranges::unary_transform_result;

  using std::ranges::transform;

}// namespace ranges

using std::replace;
using std::replace_if;

namespace ranges {
  using std::ranges::replace;
  using std::ranges::replace_if;
}// namespace ranges

using std::replace_copy;
using std::replace_copy_if;

namespace ranges {
  using std::ranges::replace_copy;
  using std::ranges::replace_copy_if;
  using std::ranges::replace_copy_if_result;
  using std::ranges::replace_copy_result;
}// namespace ranges

// [alg.fill], fill
using std::fill;
using std::fill_n;

namespace ranges {
  using std::ranges::fill;
  using std::ranges::fill_n;
}// namespace ranges

// [alg.generate], generate
using std::generate;
using std::generate_n;

namespace ranges {
  using std::ranges::generate;
  using std::ranges::generate_n;
}// namespace ranges

// [alg.remove], remove
using std::remove;
using std::remove_if;

namespace ranges {
  using std::ranges::remove;
  using std::ranges::remove_if;
}// namespace ranges

using std::remove_copy;
using std::remove_copy_if;
namespace ranges {
  using std::ranges::remove_copy;
  using std::ranges::remove_copy_if;
  using std::ranges::remove_copy_if_result;
  using std::ranges::remove_copy_result;
}// namespace ranges

// [alg.unique], unique
using std::unique;

namespace ranges {
  using std::ranges::unique;
}

using std::unique_copy;

namespace ranges {
  using std::ranges::unique_copy;
  using std::ranges::unique_copy_result;
}// namespace ranges

// [alg.reverse], reverse
using std::reverse;

namespace ranges {
  using std::ranges::reverse;
}

using std::reverse_copy;

namespace ranges {
  using std::ranges::reverse_copy;
  using std::ranges::reverse_copy_result;
}// namespace ranges

// [alg.rotate], rotate
using std::rotate;

namespace ranges {
  using std::ranges::rotate;
}

using std::rotate_copy;

namespace ranges {
  using std::ranges::rotate_copy;
  using std::ranges::rotate_copy_result;
}// namespace ranges

// [alg.random.sample], sample
using std::sample;

namespace ranges {
  using std::ranges::sample;
}

// [alg.random.shuffle], shuffle
using std::shuffle;

namespace ranges {
  using std::ranges::shuffle;
}

// [alg.shift], shift
using std::shift_left;

namespace ranges {
  // using std::ranges::shift_left;
}

using std::shift_right;

namespace ranges {
  // using std::ranges::shift_right;
}

// [alg.sorting], sorting and related operations
// [alg.sort], sorting
using std::sort;

namespace ranges {
  using std::ranges::sort;
}

using std::stable_sort;

namespace ranges {
  using std::ranges::stable_sort;
}

using std::partial_sort;

namespace ranges {
  using std::ranges::partial_sort;
}
using std::partial_sort_copy;

namespace ranges {
  using std::ranges::partial_sort_copy;
  using std::ranges::partial_sort_copy_result;
}// namespace ranges

using std::is_sorted;
using std::is_sorted_until;

namespace ranges {
  using std::ranges::is_sorted;
  using std::ranges::is_sorted_until;
}// namespace ranges

// [alg.nth.element], Nth element
using std::nth_element;

namespace ranges {
  using std::ranges::nth_element;
}

// [alg.binary.search], binary search
using std::lower_bound;

namespace ranges {
  using std::ranges::lower_bound;
}

using std::upper_bound;

namespace ranges {
  using std::ranges::upper_bound;
}

using std::equal_range;

namespace ranges {
  using std::ranges::equal_range;
}

using std::binary_search;

namespace ranges {
  using std::ranges::binary_search;
}

// [alg.partitions], partitions
using std::is_partitioned;

namespace ranges {
  using std::ranges::is_partitioned;
}

using std::partition;

namespace ranges {
  using std::ranges::partition;
}

using std::stable_partition;

namespace ranges {
  using std::ranges::stable_partition;
}

using std::partition_copy;

namespace ranges {
  using std::ranges::partition_copy;
  using std::ranges::partition_copy_result;
}// namespace ranges

using std::partition_point;

namespace ranges {
  using std::ranges::partition_point;
}
// [alg.merge], merge
using std::merge;
namespace ranges {
  using std::ranges::merge;
  using std::ranges::merge_result;
}// namespace ranges

using std::inplace_merge;

namespace ranges {
  using std::ranges::inplace_merge;
}

// [alg.set.operations], set operations
using std::includes;
namespace ranges {
  using std::ranges::includes;
}

using std::set_union;

namespace ranges {
  using std::ranges::set_union;
  using std::ranges::set_union_result;
}// namespace ranges

using std::set_intersection;
namespace ranges {
  using std::ranges::set_intersection;
  using std::ranges::set_intersection_result;
}// namespace ranges

using std::set_difference;

namespace ranges {
  using std::ranges::set_difference;
  using std::ranges::set_difference_result;
}// namespace ranges

using std::set_symmetric_difference;

namespace ranges {
  using std::ranges::set_symmetric_difference_result;

  using std::ranges::set_symmetric_difference;
}// namespace ranges

// [alg.heap.operations], heap operations
using std::push_heap;

namespace ranges {
  using std::ranges::push_heap;
}

using std::pop_heap;

namespace ranges {
  using std::ranges::pop_heap;
}

using std::make_heap;

namespace ranges {
  using std::ranges::make_heap;
}

using std::sort_heap;

namespace ranges {
  using std::ranges::sort_heap;
}

using std::is_heap;

namespace ranges {
  using std::ranges::is_heap;
}

using std::is_heap_until;

namespace ranges {
  using std::ranges::is_heap_until;
}

// [alg.min.max], minimum and maximum
using std::min;

namespace ranges {
  using std::ranges::min;
}

using std::max;

namespace ranges {
  using std::ranges::max;
}

using std::minmax;

namespace ranges {
  using std::ranges::minmax_result;

  using std::ranges::minmax;
}// namespace ranges

using std::min_element;

namespace ranges {
  using std::ranges::min_element;
}

using std::max_element;

namespace ranges {
  using std::ranges::max_element;
}

using std::minmax_element;

namespace ranges {
  using std::ranges::minmax_element_result;

  using std::ranges::minmax_element;
}// namespace ranges
 // [alg.clamp], bounded value
using std::clamp;

namespace ranges {
  using std::ranges::clamp;
}

// [alg.lex.comparison], lexicographical comparison
using std::lexicographical_compare;

namespace ranges {
  using std::ranges::lexicographical_compare;
}

// [alg.three.way], three-way comparison algorithms
using std::lexicographical_compare_three_way;

// [alg.permutation.generators], permutations
using std::next_permutation;

namespace ranges {
  using std::ranges::next_permutation_result;

  using std::ranges::next_permutation;
}// namespace ranges

using std::prev_permutation;

namespace ranges {
  using std::ranges::prev_permutation_result;

  using std::ranges::prev_permutation;
}// namespace ranges

}// namespace std

// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

export namespace std {

using std::double_t _LIBCPP_USING_IF_EXISTS;
using std::float_t _LIBCPP_USING_IF_EXISTS;

using std::acos _LIBCPP_USING_IF_EXISTS;
using std::acosf _LIBCPP_USING_IF_EXISTS;
using std::acosl _LIBCPP_USING_IF_EXISTS;

using std::asin _LIBCPP_USING_IF_EXISTS;
using std::asinf _LIBCPP_USING_IF_EXISTS;
using std::asinl _LIBCPP_USING_IF_EXISTS;

using std::atan _LIBCPP_USING_IF_EXISTS;
using std::atanf _LIBCPP_USING_IF_EXISTS;
using std::atanl _LIBCPP_USING_IF_EXISTS;

using std::atan2 _LIBCPP_USING_IF_EXISTS;
using std::atan2f _LIBCPP_USING_IF_EXISTS;
using std::atan2l _LIBCPP_USING_IF_EXISTS;

using std::cos _LIBCPP_USING_IF_EXISTS;
using std::cosf _LIBCPP_USING_IF_EXISTS;
using std::cosl _LIBCPP_USING_IF_EXISTS;

using std::sin _LIBCPP_USING_IF_EXISTS;
using std::sinf _LIBCPP_USING_IF_EXISTS;
using std::sinl _LIBCPP_USING_IF_EXISTS;

using std::tan _LIBCPP_USING_IF_EXISTS;
using std::tanf _LIBCPP_USING_IF_EXISTS;
using std::tanl _LIBCPP_USING_IF_EXISTS;

using std::acosh _LIBCPP_USING_IF_EXISTS;
using std::acoshf _LIBCPP_USING_IF_EXISTS;
using std::acoshl _LIBCPP_USING_IF_EXISTS;

using std::asinh _LIBCPP_USING_IF_EXISTS;
using std::asinhf _LIBCPP_USING_IF_EXISTS;
using std::asinhl _LIBCPP_USING_IF_EXISTS;

using std::atanh _LIBCPP_USING_IF_EXISTS;
using std::atanhf _LIBCPP_USING_IF_EXISTS;
using std::atanhl _LIBCPP_USING_IF_EXISTS;

using std::cosh _LIBCPP_USING_IF_EXISTS;
using std::coshf _LIBCPP_USING_IF_EXISTS;
using std::coshl _LIBCPP_USING_IF_EXISTS;

using std::sinh _LIBCPP_USING_IF_EXISTS;
using std::sinhf _LIBCPP_USING_IF_EXISTS;
using std::sinhl _LIBCPP_USING_IF_EXISTS;

using std::tanh _LIBCPP_USING_IF_EXISTS;
using std::tanhf _LIBCPP_USING_IF_EXISTS;
using std::tanhl _LIBCPP_USING_IF_EXISTS;

using std::exp _LIBCPP_USING_IF_EXISTS;
using std::expf _LIBCPP_USING_IF_EXISTS;
using std::expl _LIBCPP_USING_IF_EXISTS;

using std::exp2 _LIBCPP_USING_IF_EXISTS;
using std::exp2f _LIBCPP_USING_IF_EXISTS;
using std::exp2l _LIBCPP_USING_IF_EXISTS;

using std::expm1 _LIBCPP_USING_IF_EXISTS;
using std::expm1f _LIBCPP_USING_IF_EXISTS;
using std::expm1l _LIBCPP_USING_IF_EXISTS;

using std::frexp _LIBCPP_USING_IF_EXISTS;
using std::frexpf _LIBCPP_USING_IF_EXISTS;
using std::frexpl _LIBCPP_USING_IF_EXISTS;

using std::ilogb _LIBCPP_USING_IF_EXISTS;
using std::ilogbf _LIBCPP_USING_IF_EXISTS;
using std::ilogbl _LIBCPP_USING_IF_EXISTS;

using std::ldexp _LIBCPP_USING_IF_EXISTS;
using std::ldexpf _LIBCPP_USING_IF_EXISTS;
using std::ldexpl _LIBCPP_USING_IF_EXISTS;

using std::log _LIBCPP_USING_IF_EXISTS;
using std::logf _LIBCPP_USING_IF_EXISTS;
using std::logl _LIBCPP_USING_IF_EXISTS;

using std::log10 _LIBCPP_USING_IF_EXISTS;
using std::log10f _LIBCPP_USING_IF_EXISTS;
using std::log10l _LIBCPP_USING_IF_EXISTS;

using std::log1p _LIBCPP_USING_IF_EXISTS;
using std::log1pf _LIBCPP_USING_IF_EXISTS;
using std::log1pl _LIBCPP_USING_IF_EXISTS;

using std::log2 _LIBCPP_USING_IF_EXISTS;
using std::log2f _LIBCPP_USING_IF_EXISTS;
using std::log2l _LIBCPP_USING_IF_EXISTS;

using std::logb _LIBCPP_USING_IF_EXISTS;
using std::logbf _LIBCPP_USING_IF_EXISTS;
using std::logbl _LIBCPP_USING_IF_EXISTS;

using std::modf _LIBCPP_USING_IF_EXISTS;
using std::modff _LIBCPP_USING_IF_EXISTS;
using std::modfl _LIBCPP_USING_IF_EXISTS;

using std::scalbn _LIBCPP_USING_IF_EXISTS;
using std::scalbnf _LIBCPP_USING_IF_EXISTS;
using std::scalbnl _LIBCPP_USING_IF_EXISTS;

using std::scalbln _LIBCPP_USING_IF_EXISTS;
using std::scalblnf _LIBCPP_USING_IF_EXISTS;
using std::scalblnl _LIBCPP_USING_IF_EXISTS;

using std::cbrt _LIBCPP_USING_IF_EXISTS;
using std::cbrtf _LIBCPP_USING_IF_EXISTS;
using std::cbrtl _LIBCPP_USING_IF_EXISTS;

// [c.math.abs], absolute values
using std::abs _LIBCPP_USING_IF_EXISTS;

using std::fabs _LIBCPP_USING_IF_EXISTS;
using std::fabsf _LIBCPP_USING_IF_EXISTS;
using std::fabsl _LIBCPP_USING_IF_EXISTS;

using std::hypot _LIBCPP_USING_IF_EXISTS;
using std::hypotf _LIBCPP_USING_IF_EXISTS;
using std::hypotl _LIBCPP_USING_IF_EXISTS;

// [c.math.hypot3], three-dimensional hypotenuse

using std::pow _LIBCPP_USING_IF_EXISTS;
using std::powf _LIBCPP_USING_IF_EXISTS;
using std::powl _LIBCPP_USING_IF_EXISTS;

using std::sqrt _LIBCPP_USING_IF_EXISTS;
using std::sqrtf _LIBCPP_USING_IF_EXISTS;
using std::sqrtl _LIBCPP_USING_IF_EXISTS;

using std::erf _LIBCPP_USING_IF_EXISTS;
using std::erff _LIBCPP_USING_IF_EXISTS;
using std::erfl _LIBCPP_USING_IF_EXISTS;

using std::erfc _LIBCPP_USING_IF_EXISTS;
using std::erfcf _LIBCPP_USING_IF_EXISTS;
using std::erfcl _LIBCPP_USING_IF_EXISTS;

using std::lgamma _LIBCPP_USING_IF_EXISTS;
using std::lgammaf _LIBCPP_USING_IF_EXISTS;
using std::lgammal _LIBCPP_USING_IF_EXISTS;

using std::tgamma _LIBCPP_USING_IF_EXISTS;
using std::tgammaf _LIBCPP_USING_IF_EXISTS;
using std::tgammal _LIBCPP_USING_IF_EXISTS;

using std::ceil _LIBCPP_USING_IF_EXISTS;
using std::ceilf _LIBCPP_USING_IF_EXISTS;
using std::ceill _LIBCPP_USING_IF_EXISTS;

using std::floor _LIBCPP_USING_IF_EXISTS;
using std::floorf _LIBCPP_USING_IF_EXISTS;
using std::floorl _LIBCPP_USING_IF_EXISTS;

using std::nearbyint _LIBCPP_USING_IF_EXISTS;
using std::nearbyintf _LIBCPP_USING_IF_EXISTS;
using std::nearbyintl _LIBCPP_USING_IF_EXISTS;

using std::rint _LIBCPP_USING_IF_EXISTS;
using std::rintf _LIBCPP_USING_IF_EXISTS;
using std::rintl _LIBCPP_USING_IF_EXISTS;

using std::lrint _LIBCPP_USING_IF_EXISTS;
using std::lrintf _LIBCPP_USING_IF_EXISTS;
using std::lrintl _LIBCPP_USING_IF_EXISTS;

using std::llrint _LIBCPP_USING_IF_EXISTS;
using std::llrintf _LIBCPP_USING_IF_EXISTS;
using std::llrintl _LIBCPP_USING_IF_EXISTS;

using std::round _LIBCPP_USING_IF_EXISTS;
using std::roundf _LIBCPP_USING_IF_EXISTS;
using std::roundl _LIBCPP_USING_IF_EXISTS;

using std::lround _LIBCPP_USING_IF_EXISTS;
using std::lroundf _LIBCPP_USING_IF_EXISTS;
using std::lroundl _LIBCPP_USING_IF_EXISTS;

using std::llround _LIBCPP_USING_IF_EXISTS;
using std::llroundf _LIBCPP_USING_IF_EXISTS;
using std::llroundl _LIBCPP_USING_IF_EXISTS;

using std::trunc _LIBCPP_USING_IF_EXISTS;
using std::truncf _LIBCPP_USING_IF_EXISTS;
using std::truncl _LIBCPP_USING_IF_EXISTS;

using std::fmod _LIBCPP_USING_IF_EXISTS;
using std::fmodf _LIBCPP_USING_IF_EXISTS;
using std::fmodl _LIBCPP_USING_IF_EXISTS;

using std::remainder _LIBCPP_USING_IF_EXISTS;
using std::remainderf _LIBCPP_USING_IF_EXISTS;
using std::remainderl _LIBCPP_USING_IF_EXISTS;

using std::remquo _LIBCPP_USING_IF_EXISTS;
using std::remquof _LIBCPP_USING_IF_EXISTS;
using std::remquol _LIBCPP_USING_IF_EXISTS;

using std::copysign _LIBCPP_USING_IF_EXISTS;
using std::copysignf _LIBCPP_USING_IF_EXISTS;
using std::copysignl _LIBCPP_USING_IF_EXISTS;

using std::nan _LIBCPP_USING_IF_EXISTS;
using std::nanf _LIBCPP_USING_IF_EXISTS;
using std::nanl _LIBCPP_USING_IF_EXISTS;

using std::nextafter _LIBCPP_USING_IF_EXISTS;
using std::nextafterf _LIBCPP_USING_IF_EXISTS;
using std::nextafterl _LIBCPP_USING_IF_EXISTS;

using std::nexttoward _LIBCPP_USING_IF_EXISTS;
using std::nexttowardf _LIBCPP_USING_IF_EXISTS;
using std::nexttowardl _LIBCPP_USING_IF_EXISTS;

using std::fdim _LIBCPP_USING_IF_EXISTS;
using std::fdimf _LIBCPP_USING_IF_EXISTS;
using std::fdiml _LIBCPP_USING_IF_EXISTS;

using std::fmax _LIBCPP_USING_IF_EXISTS;
using std::fmaxf _LIBCPP_USING_IF_EXISTS;
using std::fmaxl _LIBCPP_USING_IF_EXISTS;

using std::fmin _LIBCPP_USING_IF_EXISTS;
using std::fminf _LIBCPP_USING_IF_EXISTS;
using std::fminl _LIBCPP_USING_IF_EXISTS;

using std::fma _LIBCPP_USING_IF_EXISTS;
using std::fmaf _LIBCPP_USING_IF_EXISTS;
using std::fmal _LIBCPP_USING_IF_EXISTS;

// [c.math.lerp], linear interpolation
using std::lerp _LIBCPP_USING_IF_EXISTS;

// [c.math.fpclass], classification / comparison functions
using std::fpclassify _LIBCPP_USING_IF_EXISTS;
using std::isfinite _LIBCPP_USING_IF_EXISTS;
using std::isgreater _LIBCPP_USING_IF_EXISTS;
using std::isgreaterequal _LIBCPP_USING_IF_EXISTS;
using std::isinf _LIBCPP_USING_IF_EXISTS;
using std::isless _LIBCPP_USING_IF_EXISTS;
using std::islessequal _LIBCPP_USING_IF_EXISTS;
using std::islessgreater _LIBCPP_USING_IF_EXISTS;
using std::isnan _LIBCPP_USING_IF_EXISTS;
using std::isnormal _LIBCPP_USING_IF_EXISTS;
using std::isunordered _LIBCPP_USING_IF_EXISTS;
using std::signbit _LIBCPP_USING_IF_EXISTS;
}// namespace std
