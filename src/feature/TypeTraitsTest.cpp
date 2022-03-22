#include <type_traits>
#include <iostream>
#include <typeinfo>
#include <array>
#include <memory>

struct A{
    int fun();
};

template<typename T>
    struct remove_pointer_decay {
        typedef typename std::remove_pointer<T>::type type;
};

void f1() {
    typedef std::decay<const A&>::type var1;
    if (std::is_pointer<var1>::value) {
        std::cout << "std decay remove ptr success...\n";
    } else {
        std::cout << "std decay remove ptr failed...\n";
    }
 
    typedef remove_pointer_decay<const A*>::type var2;
    if (std::is_pointer<var2>::value) {
        std::cout << "remove_pointer_decay remove ptr success...\n";
    } else {
        std::cout << "remove_pointer_decay remove ptr failed...\n";
    }
}
void is_pointer() {
    // template<class T>
    // struct is_pointer_helper : std::false_type {};
    // template<class T>
    // struct is_pointer_helper<T*> : std::true_type {};
    // template<class T>
    // struct is_pointer : is_pointer_helper< typename std::remove_cv<T>::type > {};

    std::cout << std::boolalpha;
    std::cout << std::is_pointer<A>::value << '\n';
    std::cout << std::is_pointer<A *>::value << '\n';
    std::cout << std::is_pointer<A &>::value << '\n';
    std::cout << std::is_pointer<int>::value << '\n';
    std::cout << std::is_pointer<int *>::value << '\n';
    std::cout << std::is_pointer<int **>::value << '\n';
    std::cout << std::is_pointer<int[10]>::value << '\n';
    std::cout << std::is_pointer<std::nullptr_t>::value << '\n';
}

void conditional() {
    // template<bool B, class T, class F>
    // struct conditional { typedef T type; };
    // template<class T, class F>
    // struct conditional<false, T, F> { typedef F type; };

    using Type1 = std::conditional<true, int, double>::type;
    using Type2 = std::conditional<false, int, double>::type;
    using Type3 = std::conditional<sizeof(int) >= sizeof(double), int, double>::type ;
    std::cout << typeid(Type1).name() << '\n';
    std::cout << typeid(Type2).name() << '\n';
    std::cout << typeid(Type3).name() << '\n';
}


template<class A>
typename std::enable_if< std::rank<A>::value == 1 >::type
print_1d(const A& a)
{
    copy(a, a+std::extent<A>::value, std::ostream_iterator<typename std::remove_extent<A>::type>(std::cout, " "));
    std::cout << '\n';
}

void remove_extent() {
    // template<class T>
    // struct remove_extent { typedef T type; };
    // template<class T>
    // struct remove_extent<T[]> { typedef T type; };
    // template<class T, std::size_t N>
    // struct remove_extent<T[N]> { typedef T type; };
    int a[][3] = {{1,2,3},{4,5,6}};
    //  print_1d(a); // 编译时错误
    print_1d(a[1]);
}


template<class A>
void foo(const A&)
{
    typedef typename std::remove_all_extents<A>::type Type;
    std::cout << "underlying type: " << typeid(Type).name() << '\n';
}
 
void remove_all_extents() {
    // template<class T>
    // struct remove_all_extents { typedef T type;};
    
    // template<class T>
    // struct remove_all_extents<T[]> {
    //     typedef typename remove_all_extents<T>::type type;
    // };
    
    // template<class T, std::size_t N>
    // struct remove_all_extents<T[N]> {
    //     typedef typename remove_all_extents<T>::type type;
    // };
    float a1[1][2][3];
    int a2[3][2];
    float a3[1][1][1][1][2];
    double a4[2][3];
    foo(a1);
    foo(a2);
    foo(a3);
    foo(a4);
}

void rank() {
    // template<class T>
    // struct rank : public std::integral_constant<std::size_t, 0> {};
    
    // template<class T>
    // struct rank<T[]> : public std::integral_constant<std::size_t, rank<T>::value + 1> {};
    
    // template<class T, std::size_t N>
    // struct rank<T[N]> : public std::integral_constant<std::size_t, rank<T>::value + 1> {};
    std::cout << std::rank<int[1][2][3]>::value << '\n';
    std::cout << std::rank<int[][2][3][4]>::value << '\n';
    std::cout << std::rank<int>::value << '\n';

}

void integral_constant() {
    // template<class T, T v>
    // struct integral_constant {
    //     static constexpr T value = v;
    //     using value_type = T;
    //     using type = integral_constant; // 使用注入类名
    //     constexpr operator value_type() const noexcept { return value; }
    //     constexpr value_type operator()() const noexcept { return value; } // C++14 起
    // };
    typedef std::integral_constant<int, 2> two_t;
    typedef std::integral_constant<int, 4> four_t;
 //  static_assert(std::is_same<two_t, four_t>::value,
//                "two_t and four_t are not equal!"); 
//  error: static assertion failed: "two_t and four_t are not equal!"
    static_assert(two_t::value*2 == four_t::value, "2*2 != 4");
 
    enum class my_e {
       e1,
       e2
    };
    typedef std::integral_constant<my_e, my_e::e1> my_e_e1;
    typedef std::integral_constant<my_e, my_e::e2> my_e_e2;
//  static_assert(my_e_e1::value == my_e::e2,
//               "my_e_e1::value != my_e::e2");
//  error: static assertion failed: "my_e_e1::value != my_e::e2"
    // static_assert(std::is_same<my_e_e1, my_e_e2>::value, "my_e_e1 != my_e_e2");
}

template <typename T, typename U>
struct decay_equiv : std::is_same<typename std::decay<T>::type, U>::type {};

void decay() {
    // template< class T >
    // struct decay {
    // private:
    //     typedef typename std::remove_reference<T>::type U;
    // public:
        // typedef typename std::conditional< 
        //     std::is_array<U>::value,
        //     typename std::remove_extent<U>::type*,
        //     typename std::conditional< 
        //         std::is_function<U>::value,
        //         typename std::add_pointer<U>::type,
        //         typename std::remove_cv<U>::type
        //     >::type
        // >::type type;
    // };
    std::cout << std::boolalpha
        << decay_equiv<int, int>::value << '\n'
        << decay_equiv<int&, int>::value << '\n'
        << decay_equiv<int&&, int>::value << '\n'
        << decay_equiv<const int&, int>::value << '\n'
        << decay_equiv<int[2], int*>::value << '\n'
        << decay_equiv<int(int), int(*)(int)>::value << '\n';
}

template<typename>
struct PM_traits {};
 
template<class T, class U>
struct PM_traits<U T::*> {
    using member_type = U;
};
 
int f();
void is_fun() {
    std::cout << std::boolalpha;
    std::cout << std::is_function<A>::value << '\n';
    std::cout << std::is_function<int(int)>::value << '\n';
    std::cout << std::is_function<decltype(f)>::value << '\n';
    std::cout << std::is_function<int>::value << '\n';
    using T = PM_traits<decltype(&A::fun)>::member_type; // T 为 int() const&
    using T1 = PM_traits<int (A::*)()>::member_type; // T 为 int() const&
    std::cout << std::is_function<T1>::value << '\n';
}


void is_array() {

// template<class T>
// struct is_array : std::false_type {};
 
// template<class T>
// struct is_array<T[]> : std::true_type {};
 
// template<class T, std::size_t N>
// struct is_array<T[N]> : std::true_type {};
    std::cout << std::boolalpha;
    std::cout << std::is_array<A>::value << '\n';
    std::cout << std::is_array<A[]>::value << '\n';
    std::cout << std::is_array<A[3]>::value << '\n';
    std::cout << std::is_array<float>::value << '\n';
    std::cout << std::is_array<int>::value << '\n';
    std::cout << std::is_array<int[]>::value << '\n';
    std::cout << std::is_array<int[3]>::value << '\n';
    std::cout << std::is_array<std::array<int, 3>>::value << '\n';

}

void remove_cv() { 
// template< class T > struct remove_cv                   { typedef T type; };
// template< class T > struct remove_cv<const T>          { typedef T type; };
// template< class T > struct remove_cv<volatile T>       { typedef T type; };
// template< class T > struct remove_cv<const volatile T> { typedef T type; };
// template< class T > struct remove_const                { typedef T type; };
// template< class T > struct remove_const<const T>       { typedef T type; };
// template< class T > struct remove_volatile             { typedef T type; };
// template< class T > struct remove_volatile<volatile T> { typedef T type; };
    typedef std::remove_cv<const int>::type type1;
    typedef std::remove_cv<volatile int>::type type2;
    typedef std::remove_cv<const volatile int>::type type3;
    typedef std::remove_cv<const volatile int*>::type type4;
    typedef std::remove_cv<int * const volatile>::type type5;
 
    std::cout << "test1 " << (std::is_same<int, type1>::value
        ? "passed" : "failed") << '\n';
    std::cout << "test2 " << (std::is_same<int, type2>::value
        ? "passed" : "failed") << '\n';
    std::cout << "test3 " << (std::is_same<int, type3>::value
        ? "passed" : "failed") << '\n';
    std::cout << "test4 " << (std::is_same<int*, type4>::value
        ? "passed" : "failed") << '\n';
    std::cout << "test5 " << (std::is_same<int*, type5>::value
        ? "passed" : "failed") << '\n';
}
 
template<class T>
typename std::result_of<T(int)>::type f(T& t)
{
    std::cout << "overload of f for callable T\n";
    return t(0);
}
 
// template<class T, class U>
// int f(U u)
// {
//     std::cout << "overload of f for non-callable T\n";
//     return u;
// }
 
void result_of() {
    struct S {
        double operator()(char, int&);
        float operator()(int) { return 1.0;}
    };
    std::result_of<S(char, int&)>::type d = 3.14; // d 拥有 double 类型
    static_assert(std::is_same<decltype(d), double>::value, "");
    // 以 int 参数调用 S 的结果是 float
    std::result_of<S(int)>::type x = 3.14; // x 拥有 float 类型
    static_assert(std::is_same<decltype(x), float>::value, ""); 
   // result_of 能以指向成员函数的指针以如下方式使用
    struct C { 
        double Func(char, int&); 
    };
    // std::result_of<decltype(&C::Func)(C, char, int&)>::type g = 3.14;
    // static_assert(std::is_same<decltype(g), double>::value, "");
 
    struct T {
        double operator()(int) {
            return 1.1;
        };
        // double Func(int&); 
    };
    T t;
    // f<C>(1); // C++11 中可能编译失败； C++14 中调用不可调用重载
    f<T>(t); // C++11 中可能编译失败； C++14 中调用不可调用重载
}

void add_LRvalue_reference() {
    // namespace detail {
    // template <class T>
    // struct type_identity { using type = T; }; // 或使用 std::type_identity (C++20 起)
    // template <class T>
    // auto try_add_lvalue_reference(int) -> type_identity<T&>;
    // template <class T>
    // auto try_add_lvalue_reference(...) -> type_identity<T>;
    // template <class T>
    // auto try_add_rvalue_reference(int) -> type_identity<T&&>;
    // template <class T>
    // auto try_add_rvalue_reference(...) -> type_identity<T>;
    // } // namespace detail
    // template <class T>
    // struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0)) {};
    // template <class T>
    // struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)) {};

    using nonref = int;
    using lref = typename std::add_lvalue_reference<nonref>::type;
    using rref = typename std::add_rvalue_reference<nonref>::type;
 
   std::cout << std::boolalpha;
   std::cout << std::is_lvalue_reference<nonref>::value << '\n';
   std::cout << std::is_lvalue_reference<lref>::value << '\n';
   std::cout << std::is_rvalue_reference<rref>::value << '\n';
}
 
void delcval() {
    struct Default { 
        int foo() const { return 1; }
    };
    struct NonDefault {
        NonDefault() = delete;
        int foo() const { return 1; }
    };
    decltype(Default().foo()) n1 = 1;                   // n1 的类型是 int
//  decltype(NonDefault().foo()) n2 = n1;               // 错误：无默认构造函数
    decltype(std::declval<NonDefault>().foo()) n2 = n1; // n2 的类型是 int
    std::cout << "n1 = " << n1 << '\n'
              << "n2 = " << n2 << '\n';
}
int main() {
//    is_pointer();
//    conditional();
    // remove_extent();
    // remove_all_extents();
    // rank();
    // integral_constant();
    // decay();
    // is_fun();
    // is_array();
    // remove_cv();
    // result_of();
    // add_LRvalue_reference();
    // delcval();
    struct s {};
    auto p1 = std::make_unique<s>();
    auto p2 = std::move(p1);
    std::cout<<(p1 == nullptr)<<std::endl;
}