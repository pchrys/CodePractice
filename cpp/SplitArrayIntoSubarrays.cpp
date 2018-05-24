#include <numeric>
#include <algorithm>
#include <string>
#include <stdexcept>

int SplitArrayIntoSubArrays(const std::vector<int>& ivec, int m)
{

    if (ivec.size() < m || ivec.size() == 0)
    {
        throw std::invalid_argument(std::string("wrong input parameter, ivec.size(): ") + std::to_string(ivec.size()) + ", m:" +
                                    std::to_string(m));
    }

    if (ivec.size() == m)
    {
        return *std::max_element(ivec.begin(), ivec.end());
    }

    std::vector<int> psum(ivec.size());
    std::partial_sum(ivec.begin(), ivec.end(), psum.begin());

    const double cParitionAvg = static_cast<double>(psum.back()) / m;


    std::vector<std::pair<int, int>> subArrays;

    int i = 0;
    int subArraySum = 0;
    for (int j = 0; j < ivec.size();)
    {
        if (ivec.size() - j + subArrays.size() == m)
        {
            // make sure we have m subarrays;
            for (int k = j; k < ivec.size(); ++k)
            {
                subArrays.push_back(std::make_pair(k, k + 1));
            }
            break;
        }

        do
        {
            ++j;
            if (i == 0)
            {
                subArraySum = psum[j - 1];
            }
            else
            {
                subArraySum = psum[j - 1] - p[i - 1];
            }

            if (ivec.size() - j + subArrays.size() <= m)
            {
                break;
            }

        } while (subArraySum < cParitionAvg);


        double delta1 = subArraySum - cParitionAvg;
        double delta2 = cParitionAvg - (subArraySum - invec[j - 1]);

        if (std::abs(delta1) < std::abs(delta2))
        {
            subArrays.push_back(std::make_pair(i, j));
            i = j;
        }
        else
        {

            subArrays.push_back(std::make_pair(i, j - 1));
            i = j - 1;
        }
    }

    std::vector<int> stk;

    // rebalance subArrays
    for (int i = 0; i < subArrays.size() - 1; ++i)
    {

        auto& p1 = subArrays[i];
        int s1 = 0;
        if (i == 0)
        {
            s1 = psum[p1.second - 1];
        }
        else
        {
            s1 = psum[p1.second - 1] - psum[p1.first - 1];
        }

        int s2;
        auto& p2 = subArrays[i + 1];
        s2 = psum[p2.second - 1] - psum[p2.first - 1];

        int k = p2.first;
        int u = k; // new bondary;
        // invariant k is the start of the second subarray;
        if (s1 < s2)
        {
            while (s1 < s2)
            {
                s1 = s1 + ivec[k];
                s2 = s2 - ivec[k];
                k++;
            }

            int delta1 = s1 - s2;
            int delta2 = delta1 - 2 * ivec[k];
            if (std::abs(delta1) < std::abs(delta2))
            {
                u = k;
            }
            else
            {
                u = k - 1;
            }
        }
        else if (s1 > s2)
        {
            while (s1 > s2)
            {
                s1 = s1 - ivec[k];
                s2 = s2 + ivec[k];
                k--;
            }

            int delta1 = s1 - s2;
            int delta2 = delta1 + 2 * ivec[k];
            if (std::abs(delta1) < std::abs(delta2))
            {
                u = k;
            }
            else
            {
                u = k + 1;
            }
        }

        if (u != p2.first)
        {
            p1.second = u;
            p2.first = u;
            stk.push_back(i + 1);
            i++; //skip one
        }
    }

    while (!stk.empty())
    {
    }
}
