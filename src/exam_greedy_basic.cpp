//
// Created by weiwei on 2021/5/10.
//

#include "greedy_basic.h"
#include "dyn_bgraph_mgr.h"
#include "lifespan_generator.h"

int main(){
    int L=10;
    int budget=10;
    int num_samples=10;

    double lmd=0.01;
    GreedyBasic<DynBGraphMgr> greedy(L,budget,num_samples);

    LifespanGenerator lifegen(L,1-exp(-lmd));

    std::string filename = "Gowalla_Checkins.txt";
    std::ifstream data(filename);
    std::string oneline;
    int x=0;
    SocialAcs sc;
    while(getline(data,oneline)){
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while(read_str>>item)
            temp.push_back(int(item));
        SocialAc soca=std::make_pair(std::make_pair(temp[0],temp[1]),temp[2]);
        sc.push_back(soca);
        x++;
        if(x==300)
            break;
    }
    for(auto &s:sc){
        std::vector<int> lifespan=lifegen.getLifespans(num_samples);
        ISetSegments segs(lifespan);
        greedy.update(s,segs);

        double greedy_basic=greedy.getResult();
        std::cout<<greedy_basic<<std::endl;
        greedy.next();
    }

    return 0;
}