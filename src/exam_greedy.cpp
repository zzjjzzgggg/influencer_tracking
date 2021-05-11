//
// Created by weiwei on 2021/4/29.
//

#include "greedy_alg.h"
#include "iset_generator.h"
#include "dyn_bgraph_mgr.h"

int main(){
    int budget=10;
    int num_samples=10;
    double p=0.6;

    GreedyAlg<DynBGraphMgr> greedy{num_samples,budget};
    std::string filename="Gowalla_Checkins.txt";

//    std::string filename="sx-stackoverflow-c2a.txt";
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
        if(x==100)
            break;
    }

    for(auto &s:sc){
        ISetGenerator isgen(num_samples,p);
        ISet is=isgen.getISet();

        greedy.update(s,is);

        double greedy_gain=greedy.getResult();
        std::cout<<"greedy:"<<greedy_gain<<std::endl;
        greedy.clear();
    }
    return 0;
}

