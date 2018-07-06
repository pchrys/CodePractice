#include <list>
#include <iterator>
#include <iostream>
#include <vector>
#include <sstream>
             
int main()
{
    // std::list<int> li{1, 2, 3, 4, 5};
    // std::list<int> li2{10, 20, 30, 40};
    // std::list<int> li3;

    // auto it = li2.begin();
    // std::advance(it, 2);
    // std::copy(li.begin(), li.end(), std::inserter(li2, it));

    // for (auto it = li2.begin(); it != li2.end(); ++it)
    // {
    //     printf("%d ", *it);
    // }

    // printf("\n");
    // std::copy(li2.begin(), li2.end(), std::ostream_iterator<int>(std::cout, " "));

    // std::cout << std::endl;
    //std::istream_iterator<int> iit(std::cin);
    std::istream_iterator<int> end;

    //printf("pleae input: %d \n", *iit);

    // std::vector<int> ivec(iit, end);

    // for(auto p: ivec){
    //     printf("%d ", p);
    // }
    // printf("\n");

    std::string s{"good is not bad 100 5000 "};

    std::istringstream istrm(s);
    std::string name;
    int v;
    //istrm >> name;
    //istrm >> v;

    // std::cout <<  "name: " << name << "\n";
    std::cout << "v: " << v << "\n";

    while(istrm >> name){
        std::cout <<  "name: " << name << "\n";
    }
    
    std::string s1{"100 good to know about"};
    auto num1 = std::stoi(s1);

    printf("num1: %d \n", num1);
    
    return 0;
}
