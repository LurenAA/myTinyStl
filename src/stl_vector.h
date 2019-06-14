#ifndef __VECTOR_H_
#define __VECTOR_H_

#include "__simple_alloc.h"
#include <iterator>
#include <stdexcept>

namespace XX{
template<typename T, class Allocator = allocator<T>>
class vector {
public:
  using value_type	= T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type =  std::ptrdiff_t;
  using reference = value_type&	;
  using const_reference = const value_type&	;
  using pointer = T*;
  using const_pointer	= const T*;
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private: 
  iterator start, 
    finish,
    ending;  
public:
  vector(): start(nullptr), finish(nullptr), ending(nullptr) {}
  explicit vector( size_type count );
  vector( size_type count, const_reference value);
  ~vector() {

  }
  //
  reference       operator[]( size_type pos ) {return *(start + pos); }
  const_reference operator[]( size_type pos ) const {return *(start + pos); }
  reference       at( size_type pos );
  const_reference at( size_type pos ) const; 
  size_type size() const noexcept { return static_cast<size_type>(finish - start);}
  bool empty() const noexcept { return start == finish;}
  iterator begin() noexcept {return start;}
  const_iterator cbegin() const noexcept {return start;}
  iterator end() noexcept {return finish;}
  const_iterator cend() const noexcept {return finish;}
  reverse_iterator rbegin() noexcept {return reverse_iterator(finish);}
  const_reverse_iterator crbegin() const noexcept {return const_reverse_iterator(finish);}
  reverse_iterator rend() noexcept {return reverse_iterator(start);}
  const_reverse_iterator crend() const noexcept {return const_reverse_iterator(start);}

};

template<typename T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::at( size_type pos ) {
  if(pos < size()) {
    return start[pos];
  }
  throw std::out_of_range("in function at, param pos out of range");
}

template<typename T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::at( size_type pos ) const {
  if(pos < size()) {
    return start[pos];
  }
  throw std::out_of_range("in function at, param pos out of range");
}

template<typename T, class Allocator>
vector<T, Allocator>::vector( size_type count ){
  finish = start = Allocator().allocate(count);
  ending = start + count;
}

template<typename T, class Allocator>
vector<T, Allocator>::vector( size_type count, const_reference value) 
: vector(count)
{
  
}

}

#endif //__VECTOR_H_ 