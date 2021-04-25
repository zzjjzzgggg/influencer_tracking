//
// Created by weiwei on 2021/4/21.
//

#include "sieve_pag.h"
#include "dyn_dgraph_mgr.h"
#include "bernoulli_get.h"
#include <gflags/gflags.h>


int main(){
    int budget=10;
    double eps=0.2;
    int num_samples=3;
    double p=1;
    SievePAG pag{num_samples,budget,eps};
    SocialAcs ss;
    ss.push_back({{1,2},1619009211});
    ss.push_back({{1,4},1619009223});
//    ss.push_back({{1,2},1619009224});
    ss.push_back({{1,3},1619009222});
    ss.push_back({{2,5},1619009223});
    ss.push_back({{2,6},1619009223});
    ss.push_back({{2,7},1619009223});
//    ss.push_back({{2,8},1619009223});
//    ss.push_back({{1,3},1619009211});
//    ss.push_back({{2,3},1619009211});
//    ss.push_back({{2,3},1619009211});
//    ss.push_back({{2,3},1619009211});
//    ss.push_back({{2,3},1619009211});
//    ss.push_back({{2,3},1619009211});
//    ss.push_back({{2,3},1619009211});

   /* int n=20;
    int m=20;
    std::pair<int, int> e[100];
    std::map<std::pair<int, int>, bool> h;
    //先生成一棵树，保证连通
    for(int i = 1; i < n; ++i)
    {
        int fa = rand()%i + 1;
        e[i] = std::make_pair(fa, i + 1);
        h[e[i]] = h[std::make_pair(i + 1, fa)] = 1;
    }
     //再生成剩余的m-n+1条边
    for(int i = n; i <= m; ++i)
    {
        int x, y;
        do
        {
            x = rand()%n + 1, y = rand()%n + 1;
        }
        while(x == y || h[std::make_pair(x, y)]);
        e[i] = std::make_pair(x, y);
        h[e[i]] = h[std::make_pair(y, x)] = 1;
    }
    //随机打乱输出
*/
//    random_shuffle(e + 1, e + m + 1);
//    for(int i = 1; i <= m; ++i)
//        printf("%d %d\n", e[i].first, e[i].second);
    int timestamp=1619356230;
//    for(int i=0;i<=m;i++){
//        ss.push_back(make_pair(e[i],timestamp));
//        timestamp++;
//    }
    int i=0;
//    for(auto &s:ss){
//        std::cout<<s.first.first<<" "<<s.first.second<<std::endl;
//    }
    for(auto &s:ss){
        BernoulliSetGenerator bsgen(num_samples,p);
        BernoulliSet bs=bsgen.getBernoulliSet();

        pag.feed(s,bs);
        std::vector<int> St=pag.getResult();
        pag.clear();

        for(auto &item:St){
            std::cout<<item<<" ";
        }
        std::cout<<std::endl;
//        pag.clear();
    }

    return 0;
}