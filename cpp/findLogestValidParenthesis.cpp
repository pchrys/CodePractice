#include <string>
#include <stack>
#include <vector>
#include <tuple>
#include <limits>
#include <iostream>

std::tuple<uint32_t, uint32_t, uint32_t> findLongestValidParentthesis(const std::string& s)
{
    std::stack<int> stk;
    std::stack<std::pair<int, int>> matchingPairs;

    std::string validSymbol("()");
    int maxLen = 0;
    auto indexPair = std::make_pair(-1, -1);

    for (int i = 0; i < s.size(); i++)
    {
        auto c = s[i];
        if (c == validSymbol[0])
        {
            stk.push(i);
        }
        else
        {
            if (!stk.empty())
            {
                // matching parenthesis found, using notation[i, j), so the matching parenthesis should be
                // i, and j-1
                auto p1 = std::make_pair(stk.top(), i + 1);

                if (!matchingPairs.empty())
                {
                    auto& t1 = matchingPairs.top();
                    // expand the valid parenthesis sequence
                    if (t1.second == p1.first)
                    {
                        t1.second = p1.second;
                    }
                    else if (t1.first >= p1.first &&
                             t1.second <= p1.second) // new group contains in the old group, cannot happen for this case
                    {
                        matchingPairs.pop();
                        // check if the new grop could merge with old grop;
                        if (!matchingPairs.empty() && matchingPairs.top().second == p1.first)
                        {
                            // merge
                            matchingPairs.top().second = p1.second;
                        }
                        else
                        {
                            matchingPairs.push(p1);
                        }
                    } // new group contains old group
                    else
                    {
                        matchingPairs.push(p1);
                    }
                }
                else
                {
                    matchingPairs.push(p1);
                }

                stk.pop();

                const auto& top = matchingPairs.top();
                if (maxLen < top.second - top.first)
                {
                    maxLen = top.second - top.first;
                    indexPair = top;
                }
            }
            else
            {
                // clear all elements in the stack;
                // and this operation may improve performance
                matchingPairs = std::stack<std::pair<int, int>>();
            }
        }
    }

    // printf("valid p: (%d, %d) inside %s() at %s:%d  \n", j, i, __func__, __FILE__, __LINE__);
    return std::make_tuple(indexPair.first, indexPair.second, maxLen);
}


std::tuple<uint32_t, uint32_t, uint32_t> findLongestValidParentthesisBk(const std::string& s)
{
    std::stack<char> stk;

    std::string validSymbol("()");
    int maxLen = 0;
    auto indexRange = std::make_pair(0, 0);

    int j = 0;
    int i = 0;
    for (i = 0; i < s.size(); i++)
    {
        auto c = s[i];
        if (c == validSymbol[0])
        {
            stk.push(c);
        }
        else
        {
            if (stk.empty())
            {
                int len = i - j;
                if (maxLen < len)
                {
                    maxLen = len;
                    indexRange = std::make_pair(j, i);
                }

                printf("valid p: (%d, %d) inside %s() at %s:%d  \n", j, i, __func__, __FILE__, __LINE__);
                j = i + 1;
            }
            else
            {
                stk.pop();
            }
        }
    }

    if (stk.empty())
    {
        int len = i - j;
        if (maxLen < len)
        {
            maxLen = len;
            indexRange = std::make_pair(j, i);
        }

        printf("valid p: (%d, %d) inside %s() at %s:%d  \n", j, i, __func__, __FILE__, __LINE__);
    }


    return std::make_tuple(indexRange.first, indexRange.second, maxLen);
}

std::tuple<uint32_t, uint32_t, uint32_t> findLongestValidParentthesis2(const std::string& s)
{
    std::stack<char> stk;

    std::string validSymbol("(){}[]");
    std::string validLeftSymbol("({[");
    std::string validRightSymbol(")}]");

    int maxLen = 0;
    auto indexRange = std::make_pair(0, 0);

    uint32_t j = 0;
    uint32_t i = 0;
    for (i = 0; i < s.size(); i++)
    {
        auto c = s[i];

        if (validLeftSymbol.find(c) != std::string::npos)
        {
            stk.push(c);
        }
        else
        {
            auto pos = validRightSymbol.find(c);
            if (pos == std::string::npos)
            {
                throw std::invalid_argument("input string contains unsupported characters \n");
            }

            if (stk.empty() || stk.top() != validLeftSymbol.at(pos))
            {
                int len = i - j;
                if (maxLen < len)
                {
                    maxLen = len;
                    indexRange = std::make_pair(j, i);
                }
                j = i + 1;
            }
            else
            {
                stk.pop();
            }
        }
    }

    if (stk.empty())
    {
        if (maxLen < i - j)
        {
            maxLen = i - j;
            indexRange = std::make_pair(j, i);
        }
    }


    return std::make_tuple(indexRange.first, indexRange.second, maxLen);
}


int main()
{
    // const std::string s(")((()))");
    const std::string s("(((()))(()))()())((())){}{}[[]])");
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t ans = 0;
    // std::tie(std::ignore, std::ignore, ans) = findLongestValidParentthesis2(s);

    std::tie(i, j, ans) = findLongestValidParentthesis(s);

    printf("range = (%d, %d), len=%d \n", i, j, ans);

    auto v1 = std::numeric_limits<signed char>::max();

    auto ep = std::numeric_limits<double>::epsilon();

    auto ep2 = ep / 1024;

    std::cout << "ep:" << ep << ", ep2: " << ep2 << "\n";

    printf("v1: %d \n", (int)v1);


    unsigned char x = 0b1010'1010;
    return 0;
}
