#include "allocator.hpp"
#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;
/**
 * 对于allocator功能的简单测试
 **/ 

void show_vector(const vector<int, xstd::allocator<int>>& vec) {
  for(auto x : vec) {
    cout << x << " ";
  }
  cout << endl;
}

int main() {
  vector<int, xstd::allocator<int>> vec;
  for(int i = 0; i < 10; ++i) {
    vec.push_back(i);
  }
  show_vector(vec);
  while(vec.size()) {
    vec.erase(vec.begin());
    show_vector(vec);
  }  
  return 1;
}
