#ifndef __SIMPLE_ALLOCATOR_H_
#define __SIMPLE_ALLOCATOR_H_

#include <type_traits>
#include <exception>
#include <limits>
namespace XX
{

template <typename T>
struct allocator
{
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;
  using is_always_equal = std::true_type;
  template <class U>
  struct rebind
  {
    typedef allocator<U> other;
  };

  size_type max_size() const throw()
  {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }
  pointer address(reference x) const
  {
    return x.ptr;
  }
  void deallocate(T *p, std::size_t n)
  {
    ::operator delete(p, n);
  }
  pointer allocate(size_type n, const void *hint = nullptr)
  {
    ptr = (T*)(::operator new(n));
    return ptr;
  }
  void construct( pointer p, const_reference val ) {
    ::new((void *)p) T(val);
  }
  void destroy( pointer p ) {
    ((T*)p)->~T();
  }
  T *ptr;
};


} // namespace XX

#endif //__SIMPLE_ALLOCATOR_H_