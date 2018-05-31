#include <vector>
#include <numeric>
#include <algorithm>
#include <unordered_map>

bool searchKsum(std::unordered_map<int64_t, std::vector<size_t>>& h1,
                std::unordered_map<int64_t, std::vector<std::pair<size_t, size_t>>>& h2,
                std::vector<size_t>& sol,
                std::vector<bool>& visited,
                size_t k,
                int64_t sum);

bool SplitArrayWithSameAverage(std::vector<int>& A)
{
    if (A.size() <= 1)
    {
        if (A.size() == 0)
        {
            return true;
        }
        else
        {

            return false;
        }
    }
    std::vector<int64_t> ivec(A.size());
    std::copy(A.begin(), A.end(), ivec.begin());

    int64_t sum = std::accumulate(ivec.begin(), ivec.end(), 0);
    std::vector<std::pair<int64_t, bool>> ps(ivec.size() -1 );

    for (size_t i = 0; i < ps.size(); i++)
    {
        int64_t t = (i + 1) * sum;

        bool flag = true;
        if (t % ivec.size() != 0)
        {
            flag = false;
            ps[i] = std::make_pair(0, flag);
        }
        else
        {
            ps[i] = std::make_pair(t / ivec.size(), flag);
        }
    }
    // build up a hash table

    std::unordered_map<int64_t, std::vector<size_t>> h1;
    std::unordered_map<int64_t, std::vector<std::pair<size_t, size_t>>> h2;

    for (size_t i = 0; i < ivec.size(); i++)
    {
        h1[ivec[i]].push_back(i);
    }

    for (size_t i = 0; i < ivec.size(); i++)
    {
        for (size_t j = i + 1; j < ivec.size(); j++)
        {
            int64_t t = ivec[i] + ivec[j];

            h2[t].push_back(std::make_pair(i, j));
        }
    }

    bool possible = false;

    for (size_t i = 0; i < ps.size(); i++)
    {
        std::vector<size_t> solution;
        std::vector<bool> visited(ivec.size());
        if (ps[i].second)
        {
            // search if we can find the sum of the i+1 element is equal to ps[i].first
            size_t k = i + 1;
            int s = ps[i].first;

            printf("k: %lu, s:%d, is called at %s:%d \n", k, s, __FILE__, __LINE__);
            possible = searchKsum(h1, h2, solution, visited, k, s);
            if (possible)
            {
                for (auto e : solution)
                {
                    printf("%lu ", e);
                }
                printf("\n");

                break;
            }
        }
    }

    return possible;
}


bool searchKsum(std::unordered_map<int64_t, std::vector<size_t>>& h1,
                std::unordered_map<int64_t, std::vector<std::pair<size_t, size_t>>>& h2,
                std::vector<size_t>& sol,
                std::vector<bool>& visited,
                size_t k,
                int64_t sum)
{
    printf("k: %lu is called at %s:%d \n", k, __FILE__, __LINE__);
    bool flag = false; // not correct
    if (k > 2)
    {
        for (const auto& e : h2)
        {
            // need remove all entries which contained i, j
            const auto& idxVec = e.second;
            std::pair<size_t, size_t> idx;
            bool found = false;
            for (const auto& e : idxVec)
            {
                if (visited[e.first] || visited[e.second])
                {
                    continue;
                }

                idx = e;
                found = true;
                break;
            }

            if (!found)
            {
                continue;
            }

            sol.push_back(idx.first);
            sol.push_back(idx.second);

            visited[idx.first] = true;
            visited[idx.second] = true;

            if (e.second.empty())
            {
                h2.erase(e.first);
            }

            flag = searchKsum(h1, h2, sol, visited, k - 2, sum - e.first);

            sol.pop_back();
            sol.pop_back();
            visited[idx.first] = false;
            visited[idx.second] = false;
        }
    }

    else if (k == 2)
    {
        if (h2.count(sum) > 0)
        {
            auto it = h2.find(sum);

            for (const auto& e : it->second)
            {
                if (visited[e.first] || visited[e.second])
                {
                    continue;
                }

                //find the valid pair
                sol.push_back(e.first);
                sol.push_back(e.second);

                flag = true;
                break;
            }
        }
    }
    else if (k == 1)
    {
        if (h1.count(sum) > 0)
        {
            auto it = h1.find(sum);
            sol.push_back(it->second.back());
            flag = true;
        }
    }

    return flag;
}


int main()
{
    std::vector<int> ivec{1, 2, 3, 4, 5, 6};

    SplitArrayWithSameAverage(ivec);


    return 0;
}
