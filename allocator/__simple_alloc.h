#ifndef __SIMPLE_ALLOC_H_
#define __SIMPLE_ALLOC_H_

#include "__contruct.h"
#include "__pool_alloc.h"
#include <limits>

namespace XX{
template <typename T>
class allocator{
private:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;
  using is_always_equal = std::true_type;
  template<class U> struct rebind { typedef allocator<U> other; };
public:
  pointer address( reference x ) const { return &*x; }
  T* allocate( size_type n ) { return (T*)alloc::allocate(n); }
  void deallocate( T* p, size_type n ) { return alloc::deallocate(p, n); }
  size_type max_size() const noexcept {return std::numeric_limits<size_type>::max() / sizeof(value_type);}
  void construct( pointer p, const_reference val ) { 
    return __construct(p, val);
  }
  void destroy( pointer p ) { return __destroy(p);}
};
} // namespace XX
#endif //__SIMPLE_ALLOC_H_