#include <cstdio>

template <typename B, typename D>
struct IsDerivedFrom
{
    class yes
    {
        int a[2];
    };

    class no
    {
    };

    static yes test(B*);
    static no test(...);

    static const int value = sizeof(test(static_cast<D*>(0))) == sizeof(yes);
};


class A
{
};
class B : public A
{
    ;
};
class C
{
};

int main()
{
    int flag = IsDerivedFrom<A, >::value;

    printf("flag = %d \n", flag);

    return 0;
}
