#include <iostream>


template<typename Iterator>
struct iterator_traits { 
    typedef typename Iterator::value_type value_type;
};

template<typename T>
struct iterator_traits<T*> {
    typedef T value_type;
};

template<typename T>
class MyIter {
public:
    typedef T value_type;
    MyIter(T *p = 0) : m_ptr(p) {}
    T& operator*() const { return *m_ptr;}
private:
    T *m_ptr;
};

template<typename Iterator>
typename iterator_traits<Iterator>::value_type  func(Iterator iter) { 
    return *iter;
};

int main(int argc, const  char *argv[]) {
    MyIter<int> iter(new int(666));
    int *p = new int(5);
    std::cout<<func(p)<<std::endl;  //print=> 666
}