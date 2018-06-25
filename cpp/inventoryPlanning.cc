// recursion

// L(i)

#include <vector>

#include <string>
#include <algorithm>
#include <numeric>

void inventoryPlanning(const std::vector<int>& D)
{

    if (D.empty())
    {
        return;
    }
    const int n = D.size();

    std::vector<int> pd(n);
    std::partial_sum(D.begin(), D.end(), pd.begin());

    std::vector<int> pu(n);
    pu[n-1] = pd[n-1];

    std::vector<int> h(pd[n-1]);
    std::vector<int> u(n);

    auto C = u;
    auto v = u;
    auto L = u;
    for(int j = n-1; j>= 0; --j){

        v[j] = pu[j] - pd[j];
        double x = v[j] > 0? h[v[j]]:0;
        x+=

    }

    // partial_sum(u) >= partial_sum(D);
    // c(i) = I(u_i - m)(u_i - m)*c + I(v_i)h(v_i);
    // v_i = pu_i - pd_i
}




struct tPlayer{
    int position;
    int cost;
    int Vorp;
};

//(i, j) denote the player at ith position , jth free agent
void maximizeVorpValue(std::vector<std::vector<tPlayer>>& P, int X){
    if(P.empty() || P[0].empty()){
        return;
    }

    const int m = P.size();
    const int n = P[0].size();

    std::vector<std::vector<int>> u(m+1);
    for(auto& e: u){
        e.resize(X+1);
    }
    //u(i, j) denotes the max value by using j dollars to play i postions
    //v(i, k) the value of the free angent at ith position, jth player
    //u(i, j)= max(u(i-1, j-x) + v(i, k))
    for(int i = 0; i < m; i++){


        for(int k = 0; k <=j ; k++){
            int y = j - k ; //extra dollars

            //find the max p[i][j].value whose cost is smaller than y;
            //sort by cost, , then sort by value
            for(int j = 0; j < P[i].size(); j++){



                auto it = std::max_element()
            }
        }
    }


}
