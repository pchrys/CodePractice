#include <string>
#include <numeric>
#include <algorithm>

// maximizing the happiness
struct tNode
{
    tNode* pParent{nullptr};
    tNode* pLeft{nullptr};
    tNode* pRightSibling{nullptr};

    std::string name;
    double conviviality;

    double h0{
        std::numeric_limits<double>::min()}; // denote the totial conviviality of the tree rooted at this node, not include current node
    double h1{
        std::numeric_limits<double>::min()}; // denote the totial conviviality of the tree rooted at this node, includeing current node

    bool initialized{false};
};


// update the total conviviality of the tree rooted at p
// h0: max conviviality if we include p
// h1: max conviviality if we donot include p

void partyPlanning(tNode* p)
{
    if (p == nullptr || p->initialized)
    {
        return;
    }

    // don't include current p
    double score = 0;

    auto q = p->pLeft;
    if (q)
    {
        if (!q->initialized)
        {
            partyPlanning(q);
            q->initialized = true;
        }
        score += q->h0 > q->h1 ? q->h0 : q->h1;
    }

    q = p->pRightSibling;
    while (q)
    {
        if (!q->initialized)
        {
            partyPlanning(q);
            q->initialized = true;
        }
        score += q->h0 > p->h1 ? p->h0 : p->h1;
        q = q->pRightSibling;
    }
    p->h0 = score;

    // include current root
    score = p->conviviality;
    q = p->pRightSibling;
    while (q)
    {
        if (!q->initialized)
        {
            partyPlanning(q);
            q->initialized = true;
        }
        score += q->h0 > q->h1 ? q->h0 : q->h1;
        q = q->pRightSibling;
    }

    q = p->pLeft;
    if (q == nullptr)
    {
        return;
    }
    q = p->pLeft;

    if (q)
    {
        if (!q->initialized)
        {
            partyPlanning(q);
            q->initialized = true;
        }
        score += q->h0 > q->h1 ? q->h0 : q->h1;
    }

    p->h1 = score;
}
int getBestPlanning(tNode* p){


}

int main(){

    return 0;
}
