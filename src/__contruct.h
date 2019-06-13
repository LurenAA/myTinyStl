#ifndef __CONSTRUCT_H_
#define __CONSTRUCT_H_

#include <new>
#include <type_traits>
namespace XX
{
  template<typename T, typename U>
  void __construct(T* p, const U& val) {
    ::new((void*)p) U(val);
  }

  template<typename T>
  void __destroy_aux(T* p, std::true_type	n) {}

  template<typename T>
  void __destroy_aux(T* p, std::false_type n) {
    p->~T();
  }
  
  template<typename T>
  void __destroy(T* p) {
    __destroy_aux(p, std::is_trivially_destructible<T>());
  }

} // namespace XX

#endif // __CONSTRUCT_H_