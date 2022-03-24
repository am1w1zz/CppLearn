#include <iostream>
#include <vector>
// #include "treap.hpp"
int rand() {
    return 3;
}

int main() {
    
    std::vector<bool> usedPriority {1,1,1,1,1,0,0,0,0,0};
    int val ;
    std::cout<<(val = rand())<<std::endl;
    // while(usedPriority[val=rand()]) {
    // }
	// usedPriority[val]=true;
	return val;
}