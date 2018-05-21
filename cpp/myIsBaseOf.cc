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

template <typename B, typename D>
struct IsTheSame
{
    class yes
    {
        int a[2];
    };

    class no
    {
    };

    static no test(const B*);
    static yes test( D*);

    static const int value = sizeof(test(static_cast<B*>(0))) == sizeof(yes);
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

void foo(const B*){
    printf("drived is called \n");
}

void foo(A* ){
    printf("base is called \n");
}

int main()
{
    int flag = IsDerivedFrom<A, B>::value;

    printf("flag = %d \n", flag);

    flag = IsTheSame<A, A>::value;
    printf("flag = %d \n", flag);
    flag = IsTheSame<A, B>::value;
    printf("flag = %d \n", flag);

    B objb;

    foo(&objb);

    return 0;
}
