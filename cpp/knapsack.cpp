#include <vector>
#include <utility>
#include <algorithm>

void outputDecision(const std::vector<std::pair<int, int>>& input, const std::vector<std::vector<int>>& d, int i, int j)
{

    if (i <= 0 || j <= 0)
    {
        return;
    }

    if (d[i][j] == 1)
    {
        outputDecision(input, d, i - 1, j - input[i-1].first);
        printf(" (%d, %d) ", input[i-1].first, input[i-1].second);
    }
    else
    {
        outputDecision(input, d, i - 1, j);
    }

}

// pair<int, int> , first denotes weight, and the second for value
int knapsack(const std::vector<std::pair<int, int>>& input, int w)
{
    // W is the maximal weight you can carry

    const int n = input.size();

    std::vector<std::vector<int>> u(n + 1);

    for (auto& e : u)
    {
        e.resize(w + 1);
    }

    decltype(u) d = u;

    for (int i = 1; i < u.size(); i++)
    {
        for (int j = 1; j < u[i].size(); j++)
        {
            std::vector<int> helper;
            for (int k = 0; k <= j; k++)
            {
                int w0 = j - k;
                if (w0 >= input[i-1].first)
                {
                    int tmp = u[i - 1][k] + input[i-1].second;
                    helper.push_back(tmp);
                }
            }

            if (!helper.empty())
            {
                u[i][j] = *std::max_element(helper.begin(), helper.end());
                d[i][j] = 1;
            }
        }
    }

    outputDecision(input, d, n, w);
    return u[n][w];
}


int main()
{
    std::vector<std::pair<int, int>> input{{10, 60}, {20, 100}, {30, 120}};
    const int w = 50;
    auto v = knapsack(input, w);

    printf("\nv: %d \n", v);

    return 0;
}
