//
// Created by weiwei on 2021/5/5.
//
#include "basic_pdg.h"
#include "ISet_segment.h"
#include "lifespan_generator.h"
#include "dyn_bgraph_mgr.h"
int main(){
    int L=40;
    int budget=10;
    double eps=0.1;
    int num_samples=10;
    double p=0.6;

    double lmd=0.01;
    BasicPAG<DynBGraphMgr> basic(L,budget,eps,num_samples);

    LifespanGenerator lifegen(L,1-exp(-lmd));

    SocialAcs ss;
    ss.push_back({{1,2},1619009223});
    ss.push_back({{1,2},1619009223});
    ss.push_back({{1,3},1619009223});
    ss.push_back({{3,4},1619009223});
    ss.push_back({{4,5},1619009223});

//    ss.push_back({{1,3},1619009223});
//    ss.push_back({{1,3},1619009223});
//    ss.push_back({{1,4},1619009223});
//    std::string filename="sx-stackoverflow-c2a.txt";
    std::string filename = "output2.txt";
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

//        std::cout<<oneline<<std::endl;
//        getline(read_str,item,'\t');
        SocialAc soca=std::make_pair(std::make_pair(temp[0],temp[1]),temp[2]);
//        std::cout<<item[0]<<" ";
//        std::cout<<item[1]<<" ";
//        std::cout<<item[2]<<" ";
        sc.push_back(soca);
        x++;
        if(x==100)
            break;
    }
    for(auto &s:sc){
        std::vector<int> lifespan=lifegen.getLifespans(num_samples);
        ISetSegments segs(lifespan);
        basic.update(s,segs);

        double pag_mx=basic.getResult();
        std::cout<<pag_mx<<std::endl;
        basic.next();
    }


    return 0;
}

