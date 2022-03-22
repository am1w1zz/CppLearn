#include <iostream>

int main() {
    int x = 0;
    int y = 4;
    int a = (x + y) >> 1;
    int b = 1 + (x + y) >> 1;
    std::cout<< a  << std::endl;
    std::cout<< b << std::endl;
}