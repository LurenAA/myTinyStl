#include "simpleAllocator.h"
#include <iostream>
#include <string>

int main()
{
    XX::allocator<int> a1;   // int 的默认分配器
    int* a = a1.allocate(1);  // 一个 int 的空间
    a1.construct(a, 7);       // 构造 int
    std::cout << a[0] << std::endl;
    a1.destroy(a);
    a1.deallocate(a, 1);      // 解分配一个 int 的空间
}