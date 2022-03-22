template<class T>
class MyTemplate
{
public:
    T Member;

    void SetMember(const T& InMember);
    constexpr T GetMemebr();
};