#include<iostream>
#include<vector>
struct  A
{
    A() {
        std::cout<< "A : default cons" <<std::endl;
    };
    // A(A v) {
    //     std::cout<< "A : cons" <<std::endl;
    // }
    A(const A& v) {
        std::cout<< "A : const cons" <<std::endl;
    }
    A(A&& v) {
        std::cout<< "A : move" <<std::endl;
    }
    ~A() {
        std::cout<< "destroy" <<std::endl;
    }
};


struct B 
{
    B() {}
    B(A a) {
        std::cout<< "B : cons" <<std::endl;
    }
    // B(const A& a){
    //     std::cout<< "B : const cons" <<std::endl;
    // };
    B(A&& a){
        std::cout<< "B : move" <<std::endl;
    }; 
    ~B() {
        std::cout<< "B : move" <<std::endl;
    };
};

struct test2
{
    std::vector<int> vec1;
};


struct AA {
    int a;
    virtual void f(){
        std::cout<<"AA::f()"<<std::endl;
    };
    virtual void g() {
        std::cout<<"AA::g()"<<std::endl;
    }
};

struct BB : AA{
    int b;
    virtual void f(){
        std::cout<<"BB::f()"<<std::endl;
    };
    void g() {
        std::cout<<"BB::g()"<<std::endl;
    }
    void h() {
        std::cout<<"BB::h()"<<std::endl;
    }
};

struct CC : BB{
    int c;
    void f(){
        std::cout<<"CC::f()"<<std::endl;
    };
    void g() {
        std::cout<<"CC::g()"<<std::endl;
    }
    void h() {
        std::cout<<"CC::h()"<<std::endl;
    }
};
void f(int a) {
    std::cout<<"f"<<std::endl;
}

struct tst {
    void f(){
        f(1);
    };
};




int main() {
    // int val = 1;
    // const int val2 = 1;
    // A a1(val);
    // A a2(val2);
    // A a3(1);
    // B*  b = new B(a3);
    // A t1;  
    // std::vector<A> a;
    // a.push_back(A());
    // A a;
    // B* b = new B(a);
    // std::vector<int> a{1,2,3,4,5};
    // for(auto& i : a) {
    //     std::cout<< i << " ";
    // }
    // std::cout<<std::endl;
    // test2 t;
    // t.vec1 = a;
    //  for(auto& i : t.vec1) {
    //     std::cout<< i << " ";
    // }
    // std::cout<<std::endl;
    // a[2] =1000;
    //  for(auto& i : a) {
    //     std::cout<< i << " ";
    // }
    // std::cout<<std::endl;
    //  for(auto& i : t.vec1) {
    //     std::cout<< i << " ";
    // }
    // BB* ap = new AA();
    // ap->f();
    // ap->g();
    // BB* bp = new BB();
    // bp->f();
    // bp->g();
    // bp->h();
    // BB* cp = new CC();
    // cp->f();
    // cp->g();
    // cp->h();
    // tst a;
    // a.f();
    tst a;
    a.f();
    
}