#include <iostream>
#include <vector>

int main() {
    int a = 1;
    int b = 1;
    const int * i = &a;
    int * const j = &b;
    // *i = b; //compile error
    i  = &b;
    *j = a;
    // j = &a; //complile error

}