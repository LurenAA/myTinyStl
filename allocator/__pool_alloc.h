#ifndef __POOL_ALLOC_H_
#define __POOL_ALLOC_H_
#include <iostream>
#include "__malloc_alloc.h"

namespace XX {
template<bool thread>   //先写成模板后续针对多线程优化
class __pool_alloc {
  private: 
    enum {
      __MAXSIZE = 128,
      __BYTESIZE = 8,
      __LISTLEN =  __MAXSIZE / __BYTESIZE
    };    
    using size_type	 = std::size_t;
    using difference_type	= std::ptrdiff_t;
    union obj {
      union obj *next;
      char *p;
    };
    static obj *list[__LISTLEN];
    static char* start_free, 
      *end_free;
    static long heap_size;
    static size_type size_up(size_type n);
    static size_type get_index(size_type size);
    static void*  refill(size_type size);
    static void chunk_allocate(size_type& size);
  public:
    static void deallocate(void* p, size_type size);
    static void* allocate (size_type n);  
};

template<bool thread>
void* __pool_alloc<thread>::allocate (size_type n) {
  if(n > __MAXSIZE) {
    return __malloc_alloc<thread>::allocate(n);
  }
  size_type curSize = size_up(n);
  size_type theIndex = get_index(curSize);
  obj ** list_ptr = &list[theIndex], 
    *res = list[theIndex];
  if(!res) {
    return refill(curSize);
  }
  *list_ptr = (*list_ptr)->next;
  return res;
}

template<bool thread> 
typename __pool_alloc<thread>::obj *__pool_alloc<thread>::
  list[__pool_alloc<thread>::__LISTLEN] = {nullptr};

template<bool thread>
char* __pool_alloc<thread>::start_free = nullptr;

template<bool thread>
char* __pool_alloc<thread>::end_free = nullptr;

template<bool thread>
long __pool_alloc<thread>::heap_size = 0;

template<bool thread>
inline typename __pool_alloc<thread>::size_type 
__pool_alloc<thread>::size_up(size_type n) {
  return (n + __BYTESIZE - 1) & ~(__BYTESIZE - 1);
}

template<bool thread>
inline typename __pool_alloc<thread>::size_type 
__pool_alloc<thread>::get_index(size_type size) {
  return size / __BYTESIZE - 1;
}

template<bool thread>
void* 
__pool_alloc<thread>::refill(size_type size) {
  size_type nodeNumbers = 20,       
    needSize =  nodeNumbers * size;
  char* reverse_begin = nullptr,     //取得的内存的开始地址
    * res = nullptr;
  difference_type reserve = end_free - start_free;   
  obj ** list_ptr = &list[get_index(size)],
    *pre_one = nullptr,
    *cur_one = nullptr;
  if(reserve < size) {
    chunk_allocate(size);
    reserve = end_free - start_free;  //更新reserve值
  } 

  if(reserve >= needSize) {
    res = start_free;
    start_free += needSize;
  } else if (reserve >= size) {
    nodeNumbers = reserve / size;
    needSize = nodeNumbers * size;
    res = start_free;
    start_free += needSize;
  } 
  reverse_begin = res;
  for(int i = 0; i < nodeNumbers - 1; ++i) {
    cur_one = (obj*)(reverse_begin + size);
    if (i == 0) {
      if(*list_ptr) {
        (*list_ptr)->next = cur_one;
      } else {
        *list_ptr = cur_one;
      }
    }
    else 
      pre_one->next = cur_one;
    pre_one = cur_one; 
    reverse_begin += size;
  }
  return res;
}

template<bool thread>
void __pool_alloc<thread>::chunk_allocate(size_type& size) {
  if(start_free) {
    difference_type reserveNow = end_free - start_free;
    size_type listIndex = get_index(reserveNow);
    obj ** list_ptr = &list[listIndex];
    if(!*list_ptr) {
      *list_ptr = (obj *)(start_free);
    } else {
      (*list_ptr)->next = (obj*)(start_free);
    }
    start_free = end_free = nullptr;
  }
  obj ** list_ptr = nullptr;
  size_type needSize = 40 * size;
  start_free = (char*)malloc(needSize);
  if(start_free == 0) {
    size_type listIndex = get_index(size) + 1;
    for(int i = listIndex; i < __LISTLEN; i++) {
      list_ptr = &list[i];
      obj *res = *list_ptr;
      if(res) {
        *list_ptr = res->next;
        start_free = res->p;
        end_free = res->p + __BYTESIZE * (i + 1);
        return ;
      }
    }
    start_free = (char*)__malloc_alloc<thread>::allocate(needSize);
  } else {
    end_free = start_free + needSize;
    heap_size += needSize;
  }
}

template<bool thread>
void __pool_alloc<thread>::deallocate(void* p, size_type size) {
  if(size > __MAXSIZE) {
    return __malloc_alloc<thread>::deallocate(p, size);
  }
  size_type theIndex = get_index(size_up(size));
  obj* returnObj = (obj*)p,
   **list_ptr = &list[theIndex],
   *res = list[theIndex];
  if(res) {
    returnObj->next = res;
  }
  *list_ptr = returnObj;
}

typedef __pool_alloc<true> alloc;
}
#endif //__POOL_ALLOC_H_