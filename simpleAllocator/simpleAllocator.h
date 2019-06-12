#ifndef __SIMPLE_ALLOCATOR_H_
#define __SIMPLE_ALLOCATOR_H_

#include <type_traits>
#include <exception>
#include <limits>
#include <new>
#include <iostream>
#include <type_traits>
using std::cout;
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
  { //sgi 2.91 defalloc做法
    std::set_new_handler(0); 
    //这个函数这里相当于初始化，其他函数可能对于set_new_handler做了别的操作          
    ptr = (T*)(::operator new(n));
    if(!ptr) {
      cout << "allocator error" << std::endl;
      exit(1);
    }
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