#include <vector>
#include <utility>
#include <stdexcept>
#include <algorithm>


// total budget x;
// N position;
// P free agent player[][]
// value = sum {x_i players[i][k].second};  //second, value
// const = sum {x_i players[i][k].first};   //first, price

void outputResult(const std::vector<std::vector<std::pair<int, int>>>& players,
                  const std::vector<std::vector<std::pair<int, int>>>& d,
                  int i,
                  int j,
                  std::vector<std::pair<int, int>>& selectedPlayers,
                  int& cost)
{

    if (i < 1 || j < 1)
    {
        return;
    }

    auto r = d[i][j];

    if (r.first != -1)
    {
        selectedPlayers.push_back(r);
        outputResult(players, d, i - 1, j - players[r.first][r.second].first, selectedPlayers, cost);
        cost += players[r.first][r.second].first;
    }
    else
    {
        outputResult(players, d, i - 1, j, selectedPlayers, cost);
    }
}

int signAgent(const std::vector<std::vector<std::pair<int, int>>>& players, int budget, std::vector<std::pair<int, int>>& selectedPlayers)
{

    if (players.empty() || players[0].empty())
    {
        throw std::invalid_argument("players cannot be empty.");
    }
    // v[i][j] denotes the max value of the first i players while budget is not over j
    const auto m = players.size();
    const auto n = players[0].size();

    std::vector<std::vector<int>> v(m + 1);
    for (auto& e : v)
    {
        e.resize(budget + 1);
    }

    std::vector<std::vector<std::pair<int, int>>> d(m + 1);
    for (auto& e : d)
    {
        d.resize(budget + 1);
    }

    for (int i = 1; i < v.size(); i++)
    {
        for (int j = 1; j < v[i].size(); j++)
        {
            // v[i][j] = v[i-1][x] + (j-x)

            std::vector<std::pair<int, int>> helper;
            for (int x = 0; x < j; x++)
            {
                auto w0 = j - x;

                // find players[i-1][k] such that players[i-1][k].first <= wo with max value
                for (int k = 0; k < n; k++)
                {
                    int tmp = -1;
                    int idx = -1;
                    auto y = players[i - 1][k];
                    if (y.first <= w0)
                    {
                        if (tmp < y.second)
                        {
                            tmp = y.second;
                            idx = k;
                        }
                    }

                    if (tmp >= 0)
                    {
                        helper.push_back(std::make_pair(tmp + v[i - 1][j], idx));
                    }
                }
            }

            if (!helper.empty())
            {
                auto it = std::max_element(helper.begin(), helper.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                    return a.first < b.first;
                });

                d[i][j] = std::make_pair(i - 1, it->second);
                v[i][j] = it->first;
            }
            else
            {
                d[i][j] = std::make_pair(-1, -1); // does not sign any agent for this position
            }
        }
    }

    int cost = 0;
    outputResult(players, d, m + 1, budget, selectedPlayers, cost);
    return v[m][budget];
}


int main()
{
    std::vector<std::vector<std::pair<int, int>>> players{{{2, 3}, {4, 7}},
        {{1, 5}, {2, 4}},
        {{1, 2},{3, 2}}};



    std::vector<std::pair<int, int>> selectedPlayers;

    int budget = 20;
    signAgent(players, budget, selectedPlayers);

    return 0;

}
