#include <iostream>
#include <stdexcept>
#include "SharedPtrThread.hpp"
// template <class T>
// struct SharedPtr {
//     using  element_type = T;
//     constexpr SharedPtr() noexcept;
//     constexpr SharedPtr(std::nullptr_t) noexcept;
    
//     template<class Y>
//     explicit SharedPtr(Y* ptr);
    
//     template<class Y, class Deleter>
//     SharedPtr(Y* ptr, Deleter d);
    
//     template<class Deleter>
//     SharedPtr(std::nullptr_t ptr, Deleter d);
    
//     template<class Y, class Deleter, class Alloc>
//     SharedPtr(Y* ptr, Deleter d, Alloc alloc);
    
//     template<class Deleter, class Alloc>
//     SharedPtr(std::nullptr_t ptr, Deleter d, Alloc alloc);

//     template<class Y>
//     SharedPtr(const SharedPtr<Y>& r, element_type* ptr) noexcept;

//     template<class Y>
//     SharedPtr(SharedPtr<Y>&& r, element_type* ptr) noexcept;

//     SharedPtr( const SharedPtr& r ) noexcept;

//     template< class Y >
//     SharedPtr(const SharedPtr<Y>& r) noexcept;

//     SharedPtr(SharedPtr&& r) noexcept;

//     template<class Y>
//     SharedPtr(SharedPtr<Y>&& r) noexcept;

//     template<class Y>
//     explicit SharedPtr(const std::weak_ptr<Y>& r);

//     template<class Y, class Deleter>
//     SharedPtr(std::unique_ptr<Y,Deleter>&& r);


// };


int main() {
    int* e = new int();
    // DefaultDeleter d ;
    // // // std::cout<<1<<std::endl;
    new (e) int(1);
    // e = new int(1);
    std::cout<<e<<std::endl;
    std::cout<<*e<<std::endl;
    // SharedPtrImpl<int> a = SharedPtrImpl<int>(e, d, 1 );
    // SharedPtrImpl<int> a = SharedPtrImpl<int>(e, d, 1 ,2 );
    // // a.m_ptr = new int(123);
    // std::cout<<*a.m_ptr<<std::endl;

    

}