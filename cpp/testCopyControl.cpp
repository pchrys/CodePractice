
#include <cstdio>
#include <memory>
#include <thread>
#include <chrono>

class tNode
{
public:
    tNode(int b = 10)
    {
        a = b;
        printf("a: %d, default constructor %s() is called at %s:%d \n", a, __func__, __FILE__, __LINE__);
    }

    tNode(const tNode& node)
    {
        a = node.a;
        printf("a: %d, copy constructor %s() is called at %s:%d \n", a, __func__, __FILE__, __LINE__);
    }

    tNode& operator=(const tNode& node)
    {
        a = node.a;
        printf("a: %d, copy assignment %s() is called at %s:%d \n", a, __func__, __FILE__, __LINE__);
    }

    tNode(tNode&& node)
    {
        a = node.a;
        printf("a: %d, move constructor %s() is called at %s:%d \n", a, __func__, __FILE__, __LINE__);
    }

    tNode& operator=(tNode&& node)
    {
        a = node.a;
        printf("a: %d, move assignment %s() is called at %s:%d \n", a, __func__, __FILE__, __LINE__);
    }

    ~tNode() { printf("a: %d, destructor %s() is called at %s:%d \n", a, __func__, __FILE__, __LINE__); }

private:
    int a = 0;
};

tNode foo()
{
    tNode node;
    return node;
}

tNode foo2()
{
    std::unique_ptr<tNode> up = std::make_unique<tNode>(20);
    return *up;
}

int main()
{
    {
        tNode n1 = foo();
        tNode n2 = foo2();
    }

    // we pause here to watch how objects are created, copied/moved, and destroyed.
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
