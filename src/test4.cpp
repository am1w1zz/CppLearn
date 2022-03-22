#include<iostream>

template <class T>
inline T* allocate(ptrdiff_t size, T*) {
    std::cout<<"allocate here"<<std::endl;
    std::set_new_handler(0);   // 为了卸载目前的内存分配异常处理函数，强制C++在内存不够的时候抛出std:bad_alloc。
    // 申请size个T类型大小的空间
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (tmp == 0) {
	std::cout << "out of memory" << std::endl; 
	exit(1);
    }
    return tmp;
}


template <class T>
inline void deallocate(T* buffer) {
    std::cout<<"deallocate here"<<std::endl;
    ::operator delete(buffer);
}

template <class T>
class all {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    pointer allocate(size_type n) { 
	return ::allocate((difference_type)n, (pointer)0);
    }
    void deallocate(pointer p) { ::deallocate(p); }
    pointer address(reference x) { return (pointer)&x; }
    const_pointer const_address(const_reference x) { 
	return (const_pointer)&x; 
    }
    size_type init_page_size() { 
	return std::max(size_type(1), size_type(4096/sizeof(T))); 
    }
    size_type max_size() const { 
	return std::max(size_type(1), size_type(UINT_MAX/sizeof(T))); 
    }
};

int main() {
    all<int> a;
    auto x = a.allocate(10);
    a.deallocate(x);
}