#include <stdexcept>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <ratio>
#include <iostream>
#include <iterator>

void getVector1(int n)
{
    if (n < 0)
    {
        throw std::invalid_argument(std::string("negative argument n:") + std::to_string(n));
    }

    auto tp1 = std::chrono::steady_clock::now();

    std::vector<int> ivec(n);
    //int i = 0;
    // for (auto& x : ivec)
    // {
    //     x = ++i;
    // }
    std::iota(ivec.begin(), ivec.end(), 1);

    auto tp2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> dd = tp2 - tp1;

    //std::copy(ivec.begin(), ivec.end(), std::ostream_iterator<int>(std::cout, " "));
    printf("spend %6.2f us time to create: %d elements vector inside %s() at %s:%d \n", dd.count(), n, __func__, __FILE__, __LINE__);
}

void getVector2(int n)
{
    if (n < 0)
    {
        throw std::invalid_argument(std::string("negative argument n:") + std::to_string(n));
    }

    auto tp1 = std::chrono::steady_clock::now();
    std::vector<int> ivec;

    for (int i = 0; i < n; ++i)
    {
        ivec.push_back(i);
    }

    auto tp2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> dd = tp2 - tp1;

    printf("spend %6.2f us time to create: %d elements vector inside %s() at %s:%d \n", dd.count(), n, __func__, __FILE__, __LINE__);
}

void getVector3(int n)
{
    if (n < 0)
    {
        throw std::invalid_argument(std::string("negative argument n:") + std::to_string(n));
    }


    auto tp1 = std::chrono::steady_clock::now();
    std::vector<int> ivec;
    ivec.reserve(n);

    for (int i = 0; i < n; ++i)
    {
        ivec.push_back(i);
    }

    auto tp2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> dd = tp2 - tp1;

    printf("spend %6.2f us time to create: %d elements vector inside %s() at %s:%d \n", dd.count(), n, __func__, __FILE__, __LINE__);
}

void getVector4(int n)
{
    if (n < 0)
    {
        throw std::invalid_argument(std::string("negative argument n:") + std::to_string(n));
    }

    auto tp1 = std::chrono::steady_clock::now();
    auto pvec = new int[n]{};

    std::iota(pvec, pvec+n, 1);
    // for (int i = 0; i < n; ++i)
    // {
    //     pvec[i] = i;
    // }

    auto tp2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> dd = tp2 - tp1;

    delete[] pvec;
    printf("spend %6.2f us time to create: %d elements vector inside %s() at %s:%d \n", dd.count(), n, __func__, __FILE__, __LINE__);
}

int main()
{
    int n = 10000000;
    //int n = 10;

    getVector1(n);
    getVector2(n);
    getVector3(n);
    getVector4(n);

    return 0;
}
