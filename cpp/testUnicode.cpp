#include <cstdio>
#include <vector>


int main()
{
    std::vector<char> degree(8, 0x0);
    snprintf(degree.data(), degree.size() - 1, "%c", 0x00B0);

    printf("degree: %s \n", degree.data());

    return 0;
}



#if 0

My understanding is that a move constructor is called if we return a object from a function if the move constructor exists.
However, I ran into a situation where the copy constructor was called instead, as shown in the following example. Why did that happen?

===========================================================

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

===============================
The above code is compiled with  g++ --std=c++17 -fno-elide-constructors
and the output is

a: 10,  tNode() is called at testCopyControl.cpp:13
a: 10,  move constructor tNode() is called at testCopyControl.cpp:31
a: 10, destructor ~tNode() is called at testCopyControl.cpp:40
a: 10,  move constructor tNode() is called at testCopyControl.cpp:31
a: 10, destructor ~tNode() is called at testCopyControl.cpp:40
a: 20,  tNode() is called at testCopyControl.cpp:13
a: 20,  copy constructor tNode() is called at testCopyControl.cpp:19
a: 20, destructor ~tNode() is called at testCopyControl.cpp:40
a: 20,  move constructor tNode() is called at testCopyControl.cpp:31
a: 20, destructor ~tNode() is called at testCopyControl.cpp:40
a: 20, destructor ~tNode() is called at testCopyControl.cpp:40
a: 10, destructor ~tNode() is called at testCopyControl.cpp:40


From the output, we know that a copy constructor is called when foo2() return *up to initialize a temporary tNode object; why did the move constructor not get called instead?

#endif
