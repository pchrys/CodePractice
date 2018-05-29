#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <iterator>


void printSolution(const std::vector<std::vector<int>>& decision, int i, int j)
{
    if (i >= j)
    {
        printf("A_%i", i);
        return;
    }

    printf("(");
    int k = decision[i][j];
    printSolution(decision, i, k);
    printSolution(decision, k + 1, j);
    printf(")");
}

void printSolution(const std::vector<std::vector<int>>& decision)
{
    for (auto& e : decision)
    {

        for (auto e1 : e)
        {
            printf("%d ", e1);
        }

        printf("\n");
    }

    // return;
    int n = decision.size();
    printSolution(decision, 0, n - 1);
}


int matrixMultiplication(const std::vector<int>& dim)
{
    if (dim.size() < 2)
    {
        throw std::invalid_argument(std::string("wrong agumentment, dim.size() = ") + std::to_string(dim.size()));
    }


    const int n = dim.size() - 1;

    std::vector<std::vector<int>> mulCost(n);
    std::vector<std::vector<int>> decision(n);

    for (auto& e : mulCost)
    {
        e.resize(n);
    }

    for (auto& e : decision)
    {
        e.resize(n);
    }

    for (int i = 0; i < n; i++)
    {
        mulCost[i][i] = 0;
    }

    // i denote the ith matrix
    for (int p = 1; p < n; p++)
    {
        for (int i = 0; i < n - p; i++)
        {
            int j = i + p;
            // compute mulCost[i][j];
            std::vector<int> helper;
            for (int k = i; k < j; k++)
            {
                int tmp = dim[i] * dim[k + 1] * dim[j + 1];
                tmp += mulCost[i][k] + mulCost[k + 1][j];

                helper.push_back(tmp);
            }

            auto it = std::min_element(helper.begin(), helper.end());
            mulCost[i][j] = *it;

            decision[i][j] = i + std::distance(helper.begin(), it);
        }
    }

    // print out optimalsolution
    printSolution(decision);
}


int main()
{

    std::vector<int> dim{2, 3, 4, 5, 6};

    matrixMultiplication(dim);

    return 0;
}
