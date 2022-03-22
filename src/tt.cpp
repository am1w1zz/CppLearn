#include<iostream>
#include <vector>
#include "tt.h"


struct aa {
    void f(int x ,int y) {
        std::cout<<"void"<<std::endl;
        return ;
    }

    void f(int x,int y )const {
        std::cout<<"int"<<std::endl;
        return ; 
    }
    ~aa() {
        std::cout<<"aa"<<std::endl;
    }
};

struct bb : aa {
    ~bb(){
        std::cout<<"bb"<<std::endl;
    }
};

struct cc {
    int v;
    cc(int v) :v(v) {
    }
};
void func();

union obj{
    union obj* obj;
    char _M_client_data[1];
};

struct node {
    node* next;
    int val;
};

int main() {
    // const aa a;
    // a.f(1,2);
    // bb a;
    // std::cout<<sizeof(aa);
    // f(1,2);
    // f(1,2);

    // cc a(1);
    // cc b(2);
    // cc& c = a;
    // std::cout<<c.v<<std::endl;
    // c = b;
    // std::cout<<c.v<<std::endl;
    // std::cout<<a.v<<std::endl;
    // std::cout<<b.v<<std::endl;
    // func(); //1  
    // extern int num;  
    // printf("%d",num); //2  
    // return 0;
    // MyTemplate<int> MyIntTemplate;
    // auto i = MyIntTemplate.GetMemebr();
//     obj a;
//     obj b;
//     a.obj = &b;
//     b._M_client_data[0] = 'a';
//     std::cout<<sizeof(a)<<std::endl;
//     std::cout<<sizeof(b)<<std::endl;
// }
    int a[] = {0x01020304,2019};
    int* b = a;
    char* c = (char*)&a[0];
    // printf("b+1:%d\n",*(b+1));
    // printf("c+1:%d\n",*(c));
    // return 0;

    std::vector<node*> list_node {new node(),new node(),new node(), new node()};
    node* n1 = new node();
    list_node[0]->next = n1;
    for(auto& i : list_node){
        std::cout<<i<<std::endl;
    }
    // std::cout<<list_node[0]->next<<std::endl;
    std::cout<<std::endl;
    list_node[0] = list_node[0]->next;
    std::cout<<std::endl;
    // std::cout<<list_node[0]<<std::endl;

    for(auto& i : list_node){
        std::cout<<i<<std::endl;
    }

    aa* a1 = new aa();
    bb* b1 = new bb();
    a1 = b1;

   

}


// int num = 3;
// void func(){  
//     printf("%d\n",num);
// }