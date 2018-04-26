#include <queue>
#include <utility>
#include <cstdio>
#include <functional>

namespace std
{
template <>
struct less<std::pair<int, int>>
{
    bool operator()(const std::pair<int, int>& a1, const std::pair<int, int> a2)
    {
        return (double)a1.first / a1.second < (double)a2.first / a2.second;
    }
};
}

int main()
{
    auto f = [](std::pair<int, int> a1, std::pair<int, int> a2) { return (double)a1.first / a1.second < double(a2.first) / a2.second; };

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(f)> q(f);
    // std::priority_queue<std::pair<int, int>,  decltype(f)> q(f);   //wrong
    // std::priority_queue<std::pair<int, int>> q;   //if we provide specialization of std::less, then we can use this form to construct a priority_queue


    int a[]{1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    int n = sizeof(a) / sizeof(a[0]);

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            auto p = std::make_pair(a[i], a[j]);
            q.push(p);
        }
    }

    int cnt = 0;
    while (!q.empty())
    {

        auto p = q.top();
        q.pop();

        printf("%4d/%-4d  ", p.first, p.second);
        cnt++;
        if (cnt == 10)
        {
            printf("\n");
            cnt = 0;
        }
    }

    return 0;
}
