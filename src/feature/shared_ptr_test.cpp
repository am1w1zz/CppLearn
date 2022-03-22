#include <memory>
#include <iostream>
#include <string_view>
#include <cstddef>

void shared_prt_construct() {
    struct Foo {
        Foo() { std::cout << "Foo...\n"; }
        ~Foo() { std::cout << "~Foo...\n"; }
    };
    
    struct D { 
        void operator()(Foo* p) const {
            std::cout << "Call delete from function object...\n";
            delete p;
        }
    };

     {
        std::cout << "constructor with no managed object\n";
        std::shared_ptr<Foo> sh1;
    }
 
    {
        std::cout << "constructor with object\n";
        std::shared_ptr<Foo> sh2(new Foo);
        std::shared_ptr<Foo> sh3(sh2);
        std::cout << sh2.use_count() << '\n';
        std::cout << sh3.use_count() << '\n';
    }
 
    {
        std::cout << "constructor with object and deleter\n";
        std::shared_ptr<Foo> sh4(new Foo, D());
        std::shared_ptr<Foo> sh5(new Foo, [](auto p) {
           std::cout << "Call delete from lambda...\n";
           delete p;
        });
    }
}


void shared_ptr_reset() {
     
    struct Foo {
        Foo(int n = 0) noexcept : bar(n) {
            std::cout << "Foo: constructor, bar = " << bar << '\n';
        }
        ~Foo() {
            std::cout << "Foo: destructor, bar = " << bar << '\n';
        }
        int getBar() const noexcept { return bar; }
        private:
        int bar;
    };
    std::shared_ptr<Foo> sptr = std::make_shared<Foo>(1);
    std::cout << "The first Foo's bar is " << sptr->getBar() << "\n";
 
    // 重置，交与新的 Foo 实例
    // （此调用后将销毁旧实例）
    sptr.reset(new Foo);
    std::cout << "The second Foo's bar is " << sptr->getBar() << "\n";
}

void output(std::string_view msg, int const* pInt) {
    std::cout << msg << *pInt << "\n";
};

void shared_ptr_get() {
    int* pInt = new int(42);
    std::shared_ptr<int> pShared = std::make_shared<int>(42);
    output("Naked pointer ", pInt);
    // output("Shared pointer ", pShared); // 编译错误
    output("Shared pointer with get() ", pShared.get());
    delete pInt;
}
// void operato() {
//     const std::size_t arr_size = 10;
//     std::shared_ptr<int[]> pis(new int[10]{0,1,2,3,4,5,6,7,8,9});
//     for (std::size_t i = 0; i < arr_size; i++){
//         std::cout << pis[i] << ' ';
//     }
// }

void owner_before() {
    struct Foo {
        int n1;
        int n2; 
        Foo(int a, int b) : n1(a), n2(b) {}
    };
    auto p1 = std::make_shared<Foo>(1, 2);
    std::shared_ptr<int> p2(p1, &p1->n1);// (1,2), 1
    std::shared_ptr<int> p3(p1, &p1->n2);// (1,2), 2
    std::cout<<p1.get()<<std::endl;
    std::cout<<p2.get()<<std::endl;
    // std::cout<<p3.pn<<std::endl;

 
    std::cout << std::boolalpha
              << "p2 < p3 " << (p2 < p3) << '\n'
              << "p3 < p2 " << (p3 < p2) << '\n'
              << "p2.owner_before(p3) " << p2.owner_before(p2) << '\n'
              << "p3.owner_before(p2) " << p3.owner_before(p3) << '\n';
 
    std::weak_ptr<int> w2(p2);
    std::weak_ptr<int> w3(p3);
    std::cout 
//              << "w2 < w3 " << (w2 < w3) << '\n'  // won't compile 
//              << "w3 < w2 " << (w3 < w2) << '\n'  // won't compile
              << "w2.owner_before(w3) " << w2.owner_before(w3) << '\n'
              << "w3.owner_before(w2) " << w3.owner_before(w2) << '\n';
}

void shared_from_this() {
    // struct Book: std::enable_shared_from_this<Book> // note: public inheritance
    // {
    //     std::shared_ptr<Book> getptr() {
    //         return shared_from_this();
    //     }
    // };
    struct Book // note: public inheritance
    {
        std::shared_ptr<Book> getptr() {
            return std::shared_ptr<Book>(this);
        }
    };
    auto data = std::make_shared<Book>();
    auto p = data.get();  //这时候我们只有一个Book*了，也就是this
    std::shared_ptr<Book> p1 = p->getptr();
    std::shared_ptr<Book> p2 = p->getptr();
    std::cout<<p1<<std::endl;
    std::cout<<p1.get()<<std::endl;
    std::cout<<p2.get()<<std::endl;
    std::cout<<p1.use_count()<<std::endl;
    std::cout<<p1.use_count()<<std::endl;
    std::cout<<p2.use_count()<<std::endl;
    
    // std::cout<<p1.get()<<std::endl;
}
int main()
{
//    shared_prt_construct();
//    shared_ptr_reset();
    // shared_ptr_get();
    // operato();
    // owner_before();

    // class BaseA { int a; };
    // class BaseB { double b; };
    // class Derived: public BaseA, public BaseB {};

    // std::shared_ptr<Derived> pd(new Derived);
    // std::shared_ptr<BaseB> pb(pd);
    // printf("%p %p\n", pd.get(), pb.get());
    // printf("%d %d\n", pd < pb, pb < pd);  // 0 0
    // printf("%d %d\n", pd.owner_before(pb), pb.owner_before(pd));  // 0 0

    // std::shared_ptr<void> p0(pd), p1(pb);
    // printf("%p %p\n", p0.get(), p1.get());
    // printf("%d %d\n", p0 < p1, p1 < p0);  // 1 0
    // printf("%d %d\n", p0.owner_before(p1), p1.owner_before(p0));  // 0 0

    // int* p = new int (10);
    // std::shared_ptr<int> a (p);
    // std::cout << a.get() << "\n";
    // std::cout << a << "\n";
    // std::cout << p << "\n";
    shared_from_this();

}