#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iterator>


std::tuple<bool, std::unordered_set<int>, std::unordered_set<int>> isBipartieGraph(const std::vector<int>& nodes, const std::vector<std::vector<int>>& graph)
{
    // DO BFS search,  track the number of nodes we have visited,
    // if the number of visited node is smaller then

    if (nodes.empty() || nodes.size() == 1)
    {
        std::unordered_set<int> tmp;
        if (graph.size() == 1)
        {
            tmp.insert(nodes.front());
        }
        return std::make_tuple(true, tmp, std::unordered_set<int>());
    }

    std::unordered_map<int, bool> visited;
    for(auto n: nodes){
        visited[n] = false;
    }

    std::vector<int> currentQueue;

    currentQueue.push_back(nodes.front());
    std::vector<int> nextQueue;

    std::vector<std::unordered_set<int>> partitions(2, std::unordered_set<int>());

    int currentIdx = 0;
    partitions[currentIdx].insert(0);
    while (!currentQueue.empty())
    {
        currentIdx = (currentIdx + 1) & 0x1;
        for (int i : currentQueue)
        {
            for (int j : graph[i])
            {
                if (visited[j] == false)
                {
                    visited[j] = true;
                    nextQueue.push_back(j);
                    partitions[currentIdx].insert(j);
                }
                else
                {
                    // check if j is located in the other partion
                    if (partitions[currentIdx].count(j) > 0)
                    {
                        // there exists an edge in the same partion
                        return std::make_tuple(false, partitions[0], partitions[1]);
                    }
                }
            }
        }

        currentQueue = nextQueue;
        nextQueue.clear();
    }

    return std::make_tuple(true, partitions[0], partitions[1]);
}

std::tuple<bool, std::unordered_set<int>, std::unordered_set<int>>
isBipartieGraph(const std::vector<std::vector<int>>& graph){

    std::vector<int> nodes(graph.size(),  0);
    std::unordered_set<int> q1;
    std::unordered_set<int> q2;

    while(!nodes.empty()){

        auto result = isBipartieGraph(nodes, graph);

        if(std::get<0>(result) == false)
        {
            //return {false, std::unordered_set<int>(), std::unordered_set<int>()};
            //return std::tuple<bool,std::unordered_set<int>, std::unordered_set<int>>(false, q1, q2);
            return make_tuple(false, q1, q2);
        }
        else{
            // nodes  - std::get<1>(result) - std::get<2>(result);
            auto p1 = std::get<1>(result);
            auto p2 = std::get<2>(result);

            decltype(p1) p3;
            std::set_union(p1.begin(), p1.end(), p2.begin(), p2.end(), std::inserter(p3, p3.begin()));

            for(auto i: p1){ q1.insert(i);}
            for(auto i: p2){ q2.insert(i);}

            std::vector<int> p4;
            std::set_difference(nodes.begin(), nodes.end(), p3.begin(), p3.end(), std::back_inserter(p4));
            nodes = p4;
        }
    }

    return make_tuple(true, q1, q2);
}

int main()
{
}
