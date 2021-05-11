//
// Created by weiwei on 2021/5/5.
//
#include "basic_pdg.h"
#include "iset_segment.h"
#include "lifespan_generator.h"
#include "dyn_bgraph_mgr.h"
int main(){
    int L=40;
    int budget=10;
    double eps=0.2;
    int num_samples=10;

    double lmd=0.01;
    BasicPAG<DynBGraphMgr> basic(L,budget,eps,num_samples);

    LifespanGenerator lifegen(L,1-exp(-lmd));

//    std::string filename="sx-stackoverflow-c2a.txt";
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

