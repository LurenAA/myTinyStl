#include "__pool_alloc.h"
#include "__simple_alloc.h"
using namespace std;

int main(int argv, char ** argc) {
  char *p = (char *)XX::alloc::allocate(6);
  for(int i = 0; i < 5; ++i) {
    *(p + i) = 'a';
  }
  p[5] = '\0';
  cout << p << endl;
  XX::alloc::deallocate(p, 7);

  char *p2 = (char *)XX::alloc::allocate(206);
  for(int i = 0; i < 5; ++i) {
    *(p2 + i) = 'b';
  }
  p2[5] = '\0';
  cout << p2 << endl;
  XX::alloc::deallocate(p2, 206);

  XX::allocator<int> a1;   // int 的默认分配器
  int* a = a1.allocate(1);  // 一个 int 的空间
  a1.construct(a, 7);       // 构造 int
  std::cout << a[0] << std::endl;
  a1.destroy(a);
  a1.deallocate(a, 1);      // 解分配一个 int 的空间
 

  return 0;
} 