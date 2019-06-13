#ifndef __MALLOC_ALLOC_H__
#define __MALLOC_ALLOC_H__
#include <new>
#include <iostream>

namespace XX {
template<bool thread>
class __malloc_alloc {
  private:
    using size_type	= std::size_t;
    using difference_type	= std::ptrdiff_t;
  public:
    static void* allocate(size_type n);
    static void deallocate(void* p, size_type n);
    static void deallocate(void* p);
};

template<bool thread>
void* __malloc_alloc<thread>::allocate(size_type n) {
  std::set_new_handler(0);
  void* p = nullptr;
  try {
    p = ::operator new(n);
  } catch (const std::bad_alloc& err) {
    std::cout << "error in operator new" << std::endl;
    exit(1);
  }
  return p;
}

template<bool thread>
void __malloc_alloc<thread>::deallocate(void* p, size_type n){
  ::operator delete(p, n);
}

template<bool thread>
void __malloc_alloc<thread>::deallocate(void* p){
  ::operator delete(p);
}

}

#endif //__MALLOC_ALLOC_H__