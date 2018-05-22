#include <cstdio>
#include <algorithm>
#include <vector>
#include <iterator>

void getTrappedRainWater(std::vector<int>::iterator it1, std::vector<int>::iterator it2, bool isLeftSubarray, int& sum);

int getTrappedRainWater(std::vector<int> ivec)
{
    if (ivec.size() <= 2)
    {
        return 0;
    }

    auto it = std::max_element(ivec.begin(), ivec.end());
    int sum = 0;

    getTrappedRainWater(ivec.begin(), it, true, sum);
    getTrappedRainWater(it, ivec.end(), false, sum);

    return sum;
}

void getTrappedRainWater(std::vector<int>::iterator it1, std::vector<int>::iterator it2, bool isLeftSubarray, int& sum)
{
    if (std::distance(it1, it2) < 2)
    {
        return;
    }

    decltype(it1) it;

    if (isLeftSubarray)
    {
        it = std::max_element(it1, it2);
    }
    else
    {
        it = std::max_element(std::next(it1), it2);
    }


    if (isLeftSubarray)
    {
        for (auto bit = it; bit < it2; bit++)
        {
            sum += *it - (*bit);
        }
        getTrappedRainWater(it1, it, true, sum);
    }
    else
    {
        for (auto bit = std::next(it1); bit < it; bit++)
        {
            sum += *it - (*bit);
        }
        getTrappedRainWater(it, it2, false, sum);
    }
}

int getTrappedRainWater(const std::vector& input)
{
    if (input.size() <= 2)
    {
        return 0;
    }

    // auto it = std::max_element(std::begin(A), std::end(A));
    int sum = 0;
}


void getTrappedRainWater2(int* p, int* q, bool isLeftSubarray, int& sum)
{
    if (std::distance(p, q) < 2)
    {
        return;
    }

    auto it = std::max_element(p, q);

    if (isLeftSubarray)
    {
        for (auto bit = it; it != q; bit++)
        {
            sum += *it - *bit;
        }

        getTrappedRainWater2(p, it, true, sum);
    }
    else
    {
        for (auto bit = p + 1; bit != it; bit++)
        {
            sum += *it - *bit;
        }
        getTrappedRainWater2(it, q, false, sum);
    }
}


int main()
{
    int A[] = {1, 2, 3};

    // getTrappedRainWater();
    return 0;
}
