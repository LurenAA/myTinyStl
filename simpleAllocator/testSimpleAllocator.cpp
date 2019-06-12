#include "simpleAllocator.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(int argv, char **argc)
{
    XX::allocator<int> a1;   // int 的默认分配器
    int *a = a1.allocate(1); // 一个 int 的空间
    a1.construct(a, 7);      // 构造 int
    cout << a[0] << endl;
    a1.destroy(a);
    a1.deallocate(a, 1); // 解分配一个 int 的空间
    int ia[5] = {1,2,3,4,5};
    vector<int, XX::allocator<int>> b(ia, ia+ 5);
    for(auto p: b) {
        cout << p << endl;
    }
    return 0;
}
