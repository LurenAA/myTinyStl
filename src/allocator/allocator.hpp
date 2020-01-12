#pragma once
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <new>
#include <algorithm>

namespace xstd {
  /**
   * template与inline
   * 《Effective C++》 条款30
   **/ 
  template <class T>
  inline T* allocate(ptrdiff_t size, T*) { //T推断类型大小
    std::set_new_handler(nullptr); //选择默认行为，即错误抛出 std::bad_alloc
    T* tmp = static_cast<T*>(::operator new(static_cast<size_t>(size * sizeof(T))));
    /**
     * 现在来说没有意义 https://zh.cppreference.com/w/cpp/memory/ * new/set_new_handler
     * 若在失败的分配尝试后，分配函数发现 std::get_new_handler 返回* 空指针值，则它会试图抛出 std::bad_alloc
     **/ 
    // if (tmp == 0) {
    //   cerr << "out of memory" << endl; 
    //   exit(1);
    // }
    return tmp;
  }

  template <class T>
  inline void deallocate(T* buffer) {
    ::operator delete(buffer);
  }
  #if __cplusplus == 201402L
  template <class T>
  inline void deallocate(T* buffer, size_t n) {
    ::operator delete(buffer, n);
  }
  #endif
  /**
   * allocator是对于::operator new、::operator delete
   * 的简单封装
   **/ 
  template<typename T>
  class allocator {
    public:
      typedef T value_type;
      typedef T* pointer;
      typedef const T* const_pointer;
      typedef T& reference;
      typedef const T& const_reference;
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;

      pointer allocate( size_type n) {
        return xstd::allocate(n, (pointer)0);
      }
      void deallocate(pointer p) { xstd::deallocate(p); }

      /**
       * ::operator delete(buffer, n) C++14才有的，
       * void deallocate( T* p, std::size_t n )接口是allocator标准要求的，
       * 有这个函数才能传入vector等容器，n必须和申请大小一样，这个函数只是性能更高
       * 标准：https://zh.cppreference.com/w/cpp/named_req/Allocator
       **/ 
      #if __cplusplus == 201402L
        void deallocate( T* p, std::size_t n ) {xstd::deallocate(p, n);}
      #else
        void deallocate( T* p, std::size_t n ) {xstd::deallocate(p);}
      #endif

      pointer address(reference x) { return static_cast<pointer>(&x); }
      const_pointer const_address(const_reference x) { 
        return static_cast<const_pointer>(&x); 
      }

      /**
       * 未知作用
       **/ 
      size_type init_page_size() { 
        return std::max(size_type(1), size_type(4096/sizeof(T))); 
      }
      /**
       * 能传递给 allocate() 的最大值	
       **/ 
      size_type max_size() const { 
        return std::max(size_type(1), size_type(UINT_MAX/sizeof(T))); 
      }
    private:
  };
  
  /**
   * 提供全特化版本避免void的allocator
   **/ 
  template<>
  class allocator<void> {
  public:
      typedef void* pointer;
  };

  /**
   * defalloc.h中并且在外部实现函数，
   * 而是在类的内部直接实现，即全部
   * inline
   **/ 
  // template<typename T>
  // typename allocator<T>::pointer 
  // allocator<T>::allocate( size_type n) 
  // {
  // }
}