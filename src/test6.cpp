#include<iostream>
#include<thread>
#include<chrono>
#include <vector>

struct A {
    void operator() () {
        std::cout<<"A" <<std::endl;
    }
};

template<typename ...T>
void f(std::vector<int>& v , T&&... args) {
    unsigned index = 0;
    for(auto& i : {args...} ) {
        std::cout<< i   << " "<< index++<<std::endl;
    }
}

int main() {
    // std::thread a = std::thread(A());
    // std::vector<int> v = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    // for(int i = 0; i < 64; ++i) {
    //     std::cout<< (i & 1) << "--" << ((i & 1) ^ 1 )<<std::endl;
    // }


    // for(auto& i : v) {
    //     // std::cout<< i << " " << i ^ 1 <<std::end;
    // }
    std::vector<int> a;
    f(a, "123", "456", "789");

}