#include <string>
#include <stack>
#include <vector>
#include <tuple>

std::tuple<uint32_t, uint32_t, uint32_t> findLongestValidParentthesis(const std::string& s)
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
    //const std::string s(")((()))");
    const std::string s("()())((())){}{}[[]])");
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t ans = 0;
    //std::tie(std::ignore, std::ignore, ans) = findLongestValidParentthesis2(s);

    std::tie(i, j, ans) = findLongestValidParentthesis2(s);

    printf("range = (%d, %d), len=%d \n", i, j, ans);

    return 0;
}
