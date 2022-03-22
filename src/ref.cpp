#include <iostream>
using namespace std;

void check(int&){
    cout<<"left"<<endl;
}

void check(int&&){
        cout<<"right"<<endl;
}

template <typename T>
void print(T&& v){
    check(forward<T>(v));
}
int main(){
    int x=0;
    print(x);
    print(move(x));
}