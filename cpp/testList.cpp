#include <list>
#include <iterator>
#include <iostream>

int mm = 200;
int main()
{
    std::list<int> li{1, 2, 3, 4, 5};

    auto it = li.begin();
    std::advance(it, 2);
    it = li.insert(it, {10, 20});
    std::cout << "*it: " << *it << "\n" << std::endl;

    std::copy(li.begin(), li.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
    int n = 3;
    static int m = 1000;
    // auto f = [li, n]() mutable {
    auto f = [&li, &n]() {
        for (auto i : li)
        {
            printf("%d ", i);
        }
        printf("\n");

        n = 100;
        printf("n=%d \n", n);
        printf("m = %d \n", m);
        printf("mm = %d \n", mm);

        li.emplace_back(100);
    };

    // n = -n;
    m = -m;
    mm = -mm;

    f();

    size_t v1 = 42;
    auto f2 = [v1]() mutable { return ++v1; };


    std::list<int> li2{10, 20, 30, 40};
    std::list<int> li3;
    it = li2.begin();
    std::advance(it, 2);
    std::copy(li.begin(), li.end(), std::inserter(li2, it));

    for (auto it = li2.begin(); it != li2.end(); ++it)
    {
        printf("%d ", *it);
    }

    printf("\n");

    std::copy(li2.begin(), li2.end(), std::ostream_iterator<int>(std::cout, " "));


    return 0;
}
