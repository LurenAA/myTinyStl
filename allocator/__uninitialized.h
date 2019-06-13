#ifndef _UNINITIALIZED_H__
#define _UNINITIALIZED_H__
#include <type_traits>
#include <exception>
#include "__simple_alloc.h"

namespace XX{
  template<typename InputItr, typename ForwardItr>
  ForwardItr uninitialized_copy_aux( InputItr first, InputItr last, ForwardItr d_first, std::true_type n) {
    std::copy(first, last, d_first);
    return d_first;
  }

  template<typename InputItr, typename ForwardItr>
  ForwardItr uninitialized_copy_aux( InputItr first, InputItr last, ForwardItr d_first, std::false_type n) {
    auto iterLen = last - first;
    for(decltype(iterLen) i = 0; i < iterLen; ++i) {
      try { 
        __construct( &*(d_first + i), *(first + i));  //&*用来处理迭代器的情况
      } catch(const std::exception& err) {
        __destroy(&*(d_first + i));
      } 
    }
    return d_first;
  }

  template<typename InputItr, typename ForwardItr>
  ForwardItr uninitialized_copy( InputItr first, InputItr last, ForwardItr d_first ) {
    return uninitialized_copy_aux(first, last, d_first, std::is_pod<ForwardItr>());
  }
}
#endif//_UNINITIALIZED_H__