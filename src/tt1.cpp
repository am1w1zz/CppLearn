#include "tt.h"

template<typename T>
void MyTemplate<T>::SetMember(const T& InMember)
{
    Member = InMember;
}

template<typename T>
T MyTemplate<T>::GetMemebr()
{
    return Member;
}


template class MyTemplate<int>;