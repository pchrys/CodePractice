#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

enum class Op
{
    copy = 0,
    replace,

    remove,
    insert
};

void ouputOperationSequence(const std::vector<std::vector<std::tuple<Op, int, int>>>& d, int m, int n)
{
    if (m < 1 || n < 1)
    {
        return;
    }

    int i = 0;
    int j = 0;
    Op editOp;
    std::tie(editOp, i, j) = d[m][n];
    ouputOperationSequence(d, i, j);

    std::string opString;
    switch (editOp)
    {
    case Op::copy:
        opString = "copy";
        break;
    case Op::replace:
        opString = "replace";
        break;
    case Op::remove:
        opString = "remove";
        break;
    case Op::insert:
        opString = "insert";
        break;
    }

    printf(" %s ", opString.c_str());
}
int editDistance(const std::string& x, const std::string& y, const std::vector<int>& opCost)
{
    if (x.empty() && y.empty())
    {
        return 0;
    }

    const int m = x.size();
    const int n = y.size();
    std::vector<std::vector<int>> mm(m + 1);

    for (auto& e : mm)
    {
        e.resize(n + 1);
    }
    for (size_t j = 1; j < mm[0].size(); j++)
    {
        mm[0][j] = mm[0][j - 1] + opCost[static_cast<int>(Op::insert)];
    }

    for (size_t i = 1; i < mm.size(); i++)
    {
        mm[i][0] = mm[i - 1][0] + opCost[static_cast<int>(Op::remove)];
    }

    std::vector<std::vector<std::tuple<Op, int, int>>> d(m + 1);
    for (auto& e : d)
    {
        e.resize(n + 1);
    }

    // mm[i][j] denotes the edit distance from x[i-1] to y[j-1];
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            printf("building dp table (%d, %d) at %s:%d \n", i, j, __FILE__, __LINE__);

            std::vector<std::pair<int, Op>> helper;
            if (x[i - 1] == y[j - 1])
            {
                helper.push_back(std::make_pair(mm[i - 1][j - 1] + opCost[static_cast<size_t>(Op::copy)], Op::copy)); // copy,
            }
            else
            {
                // replace
                helper.push_back(std::make_pair(mm[i - 1][j - 1] + opCost[static_cast<size_t>(Op::replace)], Op::replace));
            }

            helper.push_back(std::make_pair(mm[i- 1][j] + opCost[static_cast<size_t>(Op::remove)], Op::remove));

            helper.push_back(std::make_pair(mm[i][j - 1] + opCost[static_cast<size_t>(Op::insert)], Op::insert)); // insert

            auto it = std::min_element(
                helper.begin(), helper.end(), [](const std::pair<int, Op>& a, const std::pair<int, Op>& b) { return a.first < b.first; });

            mm[i][j] = it->first;

            switch(it->second){
            case Op::copy:
            case Op::replace:
                d[i][j] = std::make_tuple(it->second, i-1, j-1);
                break;
            case Op::remove:
                d[i][j] = std::make_tuple(it->second, i-1, j);
                break;
            case Op::insert:
                d[i][j] = std::make_tuple(it->second, i, j-1);
                break;
            default:
                printf("operation not supported. \n");
            }

        }
    }

    printf("=============d table===================\n");

    ouputOperationSequence(d, m, n);

    printf("\n=============mm table===================\n");
    for (const auto& e1 : mm)
    {
        for (const auto e2 : e1)
        {
            printf("%4d ", e2);
        }
        printf("\n");
    }


    return mm[m][n];
}

int editDistance(const std::string& x, const std::string& y)
{
    // const std::map<std::string, int> opCost {{"copy", 3},
    //                                            {"replace", 5},
    //                                            {"delete", 5},
    //                                            {"insert", 5},
    //                                            {"twiddle", 5},
    //                                            {"kill", 5}};


    std::vector<int> opCost{0, 1, 1, 1};
    return editDistance(x, y, opCost);
}


int main()
{
    const std::string x{"hor"};
    const std::string y{"or"};

    const int ed = editDistance(x, y);
    printf("ed: %d \n", ed);

    return 0;
}
