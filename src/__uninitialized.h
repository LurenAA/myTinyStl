#ifndef _UNINITIALIZED_H__
#define _UNINITIALIZED_H__
#include <type_traits>
#include <exception>
#include "__simple_alloc.h"

namespace XX{
  template<typename InputItr, typename ForwardItr>
  ForwardItr uninitialized_copy_aux( InputItr first, InputItr last, ForwardItr d_first, std::true_type n) {
    return std::copy(first, last, d_first);
  }

  template<typename InputItr, typename ForwardItr>
  ForwardItr uninitialized_copy_aux( InputItr first, InputItr last, ForwardItr d_first, std::false_type n) {
    auto iterLen = last - first;
    for(decltype(iterLen) i = 0; i < iterLen; ++i) {
      try { 
        __construct( &*(d_first + i), *(first + i));  //&*用来处理迭代器的情况
      } catch(...) {
        __destroy(&*(d_first + i));
        throw;
      } 
    }
    return d_first + iterLen;
  }

  template<typename InputItr, typename ForwardItr>
  ForwardItr uninitialized_copy( InputItr first, InputItr last, ForwardItr d_first ) {
    return uninitialized_copy_aux(first, last, d_first, std::is_pod<ForwardItr>());
  }

  template<typename OutputIt, typename Size, typename T>
  OutputIt uninitialized_fill_n_aux(OutputIt first, Size count, const T& val, std::false_type) {
    for(Size i = 0; i < count; ++i) {
      try {
        __construct(&*(first + i), val);
      } catch (...) {
        __destroy(&*(first + i));
        throw;
      }
    }
  }

  template<typename OutputIt, typename Size, typename T>
  OutputIt uninitialized_fill_n_aux(OutputIt first, Size count, const T& val, std::true_type) {
    return std::fill_n(first, count, val);
  }

  template<typename OutputIt, typename Size, typename T>
  OutputIt uninitialized_fill_n(OutputIt first, Size count, const T& val) {
    return uninitialized_fill_n_aux(first, count, val, std::is_pod<T>());
  }
}
#endif//_UNINITIALIZED_H__