读allocator源码（《STL源码剖析》），并实现一个小的demo

2019/6/9：读之前先按照cppreference的需求实现了一个基本版本
2019/6/10： 参照了sgi 2.91 defalloc 对于operator new的错误处理办法。
2019/6/12: 读完《STL源码剖析》第二章，自己实现了整个alloc，对于多线程的问题后续再来完善
2019/6/13: 完成了一个uninitialized_copy工具函数，理解了&*ptr这种对于迭代器的兼容操作