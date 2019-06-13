#ifndef __STL_ITERATOR_H_
#define __STL_ITERATOR_H_
#include <iterator>

namespace XX
{
  template< class InputIt, class Distance >
  void __advance( InputIt& it, Distance n, std::input_iterator_tag) {
    while(n-- > 0) {
      ++it;
    }
  }

  template< class InputIt, class Distance >
  void __advance( InputIt& it, Distance n, std::output_iterator_tag ) {
    __advance(it, n,  std::input_iterator_tag());
  }

  template< class InputIt, class Distance >
  void __advance( InputIt& it, Distance n, std::forward_iterator_tag  ) {
    __advance(it, n,  std::input_iterator_tag());
  }

  template< class InputIt, class Distance >
  void __advance( InputIt& it, Distance n, std::bidirectional_iterator_tag  ) {
    if(n >= 0) {
      __advance(it, n,  std::input_iterator_tag());
    } else if (n < 0) {
      while(n++ < 0) {
        --it;
      }
    }
  }

  template< class InputIt, class Distance >
  void __advance( InputIt& it, Distance n, std::random_access_iterator_tag  ) {
    it += n;
  }

  template< class InputIt, class Distance >
  void advance( InputIt& it, Distance n ) {
    __advance(it, n, std::iterator_traits<InputIt>());
  }
} // namespace XX

#endif