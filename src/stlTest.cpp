#include "__pool_alloc.h"
#include "__simple_alloc.h"
#include "__uninitialized.h"
#include "stl_vector.h"
#include <vector>
#include "stl_list.h"
using namespace std;

int main(int argv, char ** argc) {
  XX::list<int> p{8,7,6,5,4,2,1};
  p.sort();

  return 0;
} 