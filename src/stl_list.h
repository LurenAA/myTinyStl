#ifndef __STL_LIST_H_
#define __STL_LIST_H_
#include "__simple_alloc.h"
#include <iterator>
#include <initializer_list>

namespace XX{
template<typename T>
struct __list_node{
  __list_node(const T& value) : value(value) {}
  T value;
  __list_node* prev = nullptr;
  __list_node* next = nullptr;
};

template<typename T>
class __list_iterator;

template<typename T>
void __sort(__list_iterator<T> start, 
  __list_iterator<T> end);

template<typename T>
void swap(__list_iterator<T>a , __list_iterator<T>b ) {
  if(a == b) return ;
  auto tmpValue = a.node->value;
  a.node->value = b.node->value;
  b.node->value = tmpValue;
}

template<typename T>
class __list_iterator : public std::iterator<std::bidirectional_iterator_tag, T>
{
template<typename T1, typename alloc> friend class list;
friend void XX::swap<T>(__list_iterator<T>a , __list_iterator<T>b );
friend void __sort<T>(__list_iterator<T> start, 
  __list_iterator<T> end);
public:
  __list_iterator(__list_node<T>* node) : node(node) {}
  __list_iterator() = default;
  using typename std::iterator<std::bidirectional_iterator_tag, T>::value_type;
  using typename std::iterator<std::bidirectional_iterator_tag, T>::reference;
  using typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type;
  using typename std::iterator<std::bidirectional_iterator_tag, T>::iterator_category;
  using typename std::iterator<std::bidirectional_iterator_tag, T>::pointer;
  value_type operator*() { return node->value;}
  reference operator->() {return &(operator*());}
  bool operator==(const __list_iterator<T>& rhs) {return node == rhs.node;}
  bool operator!=(const __list_iterator<T>& rhs) {return !(operator==(rhs));}
  __list_iterator& operator++() {
    node = node->next;
    return *this;
  }
  __list_iterator operator++(int) {
    auto tmp = *this;
    node = node->next;
    return tmp;
  }
  __list_iterator& operator--() {
    node = node->prev;
    return *this;
  }
  __list_iterator operator--(int) {
    auto tmp = *this;
    node = node->prev;
    return tmp;
  }
private:
  __list_node<T> *node = nullptr;
};

template<typename T, typename alloc = allocator<T>>
class list {
  private:
    __list_node<T>* node;
    static allocator<__list_node<T> > staticAlloc;
    __list_node<T>* create_node(const T& value = 0);
  public:
    list();
    list(const std::initializer_list<T>& lis);
    ~list();
    typedef __list_iterator<T> iterator;
    typedef const __list_iterator<T> const_iterator;
    typedef std::size_t size_type;
    typedef T& reference;
    typedef const T& const_reference;
    iterator erase( iterator pos );
    iterator erase( iterator first, iterator last );
    void transfer(iterator pos, iterator start, iterator end);
    void merge( list& other );
    iterator begin() {return (iterator)(node->next);}
    iterator end() {return (iterator)(node);}
    const_iterator cbegin() const {return (iterator)(node->next);}
    const_iterator cend() const {return (iterator)(node);}
    size_type size() const {
      return (size_type)(std::distance(cbegin(), cend()));
    }
    bool empty() const {
      return cbegin() == cend();
    }
    iterator insert( iterator pos, const T& value );
    void push_back( const T& value ) {insert(end(), value);}
    void sort();
};

template<typename T, typename alloc>
allocator<__list_node<T> > list<T, alloc>::staticAlloc;

template<typename T, typename alloc>
__list_node<T>* list<T,alloc>::create_node(const T& value)
{
  __list_node<T>* node = staticAlloc.allocate(sizeof(__list_node<T>));
  try {
    __construct(&node->value, value);
  } catch(...) {
    __destroy(node);
    staticAlloc.deallocate(node, sizeof(__list_node<T>));
    throw;
  } 
  return node;
}

template<typename T, typename alloc>
list<T,alloc>::list() :
node(create_node())
{
  node->next = node;
  node->prev = node;
}

template<typename T, typename alloc>
typename list<T,alloc>::iterator 
list<T,alloc>::insert( iterator pos, const T& value ) 
{
  __list_node<T>* newNode = create_node(value);
  newNode->prev = pos.node->prev;
  newNode->next = pos.node;
  pos.node->prev->next = newNode;
  pos.node->prev = newNode;
  return (iterator)(newNode);
}

template<typename T, typename alloc>
list<T,alloc>::~list() {
  auto curNode = node->next;
  while(curNode != node) {
    auto deleteNode = curNode;
    curNode = curNode->next;
    __destroy(deleteNode);
    staticAlloc.deallocate(deleteNode, sizeof(__list_node<T>));
  }
  __destroy(curNode);
  staticAlloc.deallocate(curNode, sizeof(__list_node<T>));
}

template<typename T, typename alloc>
void list<T,alloc>::transfer(iterator pos, iterator start, iterator last) 
{
  if(start != last) {
    auto endOne = (iterator)last.node->prev;
    start.node->prev->next = last.node;
    last.node->prev = start.node->prev;
    auto posPrev = (iterator)pos.node->prev;
    posPrev.node->next = start.node;
    start.node->prev = posPrev.node;
    endOne.node->next = pos.node;
    pos.node->prev = endOne.node;
  }
}

template<typename T, typename alloc>
typename list<T,alloc>::iterator 
list<T,alloc>::erase( iterator pos ) 
{
  if(pos != end()) {
    pos.node->prev->next = pos.node->next;
    pos.node->next->prev = pos.node->prev;
    auto resIter = (iterator)pos.node->next;
    __destroy(pos.node);
    staticAlloc.deallocate(pos.node, sizeof(__list_node<T>));
    return resIter;
  }
  return end();
}

template<typename T, typename alloc>
typename list<T,alloc>::iterator 
list<T,alloc>::erase( iterator first, iterator last )
{
  while(first != last) {
    first = erase(first);
  }
  return first;
}

template<typename T, typename alloc>
list<T,alloc>::list(const std::initializer_list<T>& lis) 
: list()
{
  for(auto beg = lis.begin(); beg != lis.end(); ++beg) {
    push_back(*beg);
  }
}

template<typename T, typename alloc>
void list<T,alloc>::merge( list& other ) {
//必须都已经排序过
  auto beg2 = other.begin();
  for(auto beg = begin(); beg != end(); beg++) {
    while(*beg >= *beg2) {
      insert(beg, *beg2++);
      if(beg2 == other.end()) return ;
    }
    if(beg2 == other.end()) return ;
  }
  for(; beg2 != other.end(); beg2++) 
    push_back(*beg2);
}

template<typename T>
void __sort(__list_iterator<T> start, 
  __list_iterator<T> end) 
{
  if(start == end || (__list_iterator<T>)start.node->next == end) return;
  auto pivot = start,
    lhs = (__list_iterator<T>)start.node->next,
    i = lhs;
  while(i != end) {
    if(*i <= *pivot) 
      swap(i, lhs++);
    ++i;
  }
  swap(--lhs, pivot);
  __sort(start, lhs);
  __sort(++lhs, end);
}

template<typename T, typename alloc>
void list<T,alloc>::sort() {
  XX::__sort(begin(), end());
}

}

#endif //__STL_LIST_H_