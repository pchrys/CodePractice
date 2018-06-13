#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

void printSolution(const std::vector<int>& v, const std::vector<std::vector<int>>& d, int i, int j)
{
    if (i > j)
    {
        return;
    }

    int r = d[i][j];

    printSolution(v, d, i, r - 1);
    printf("%5d ", v[r]);
    printSolution(v, d, r + 1, j);
}

// int breakString(const std::vector<std::vector<std::tuple<int, int, int> >>& u,
//                 const std::string& s,
//                 const std::vector<int>& v,
//                 int i,
//                 int j,
//                 int p,
//                 int q)
// {

//     if(std::get<2>(u[i][j])  != std::numeric_limits<int>::max()){
//         return std::get<2>(u[i][j]);
//     }

//     if (p == q)
//     {
//         if (v[p] >= i && v[p] <= j)
//         {
//             return j - i + 1;
//         }
//         else
//         {
//             return 0;
//         }
//     }

//     if (i > j)
//     {
//         return 0;
//     }

//     std::vector<int> h;
//     for (int k = p; k <= q; k++)
//     {
//         int r = v[k];

//         auto t1 = breakString(u, s, v, i, r, p, k - 1);
//         auto t2 = breakString(u, s, v, r + 1, j, k + 1, q);

//         h.push_back(t1 + t2 + j - i + 1);
//     }

//         auto it = std::min_element(h.begin(), h.end());
//         u[i][j][p][q] = *it;
// }
//}

int breakString(std::vector<std::vector<std::vector<std::vector<int>>>>& u,
                const std::string& s,
                const std::vector<int>& v,
                int i,
                int j,
                int p,
                int q)
{
    if (u[i][j][p][q] != std::numeric_limits<int>::max())
    {
        return u[i][j][p][q];
    }

    if (p == q)
    {
        if (v[p] >= i && v[p] <= j)
        {
            return j - i + 1;
        }
        else
        {
            return 0;
        }
    }
    else if(p > q){
        return 0;
    }

    if (i > j)
    {
        return 0;
    }

    std::vector<int> h;
    for (int k = p; k <= q; k++)
    {
        int r = v[k];

        auto t1 = breakString(u, s, v, i, r, p, k - 1);
        auto t2 = breakString(u, s, v, r + 1, j, k + 1, q);

        h.push_back(t1 + t2 + j - i + 1);
    }

    auto it = std::min_element(h.begin(), h.end());
    u[i][j][p][q] = *it;
}

int breakString(const std::string& s, const std::vector<int>& v)
{
    const int n = s.size();
    const int m = v.size();
    if (n < 1 || m < 1)
    {
        return 0;
    }

    // u[i, j, p, q]
    std::vector<std::vector<std::vector<std::vector<int>>>> u(n);
    auto d = u;

    for (auto& e : u)
    {
        e.resize(n);
        for (auto& e1 : e)
        {
            e1.resize(m);
            for (auto& e2 : e1)
            {
                e2.resize(m, std::numeric_limits<int>::max());
            }
        }
    }


    return breakString(u, s, v, 0, n-1, 0, m-1);

}


int main()
{
    const int n = 20;
    std::string s(n, 0x0);
    int i = 'a';
    for(auto& e: s){
        e = i++;
    }

    std::vector<int> v{2, 8, 10};
    int cnt = breakString(s, v);


    printf("cnt = %d \n", cnt);
    return 0;
}
