//
// Created by weiwei on 2021/5/10.
//

#include "greedy_basic.h"
#include "dyn_bgraph_mgr.h"
#include "lifespan_generator.h"
#include "basic_pdg.h"

int main(){
    int L=40;
    int budget=10;
    double eps=0.2;
    int num_samples=10;
    double p=0.6;

    double lmd=0.01;
    GreedyBasic<DynBGraphMgr> greedy(L,budget,num_samples);
    BasicPAG<DynBGraphMgr> basic(L,budget,eps,num_samples);

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
        if(x==10000)
            break;
    }
    int temp=1;
    std::string outfile="greedy_and_basic.txt";
    std::ofstream out(outfile);
    for(auto &s:sc){
        std::vector<int> lifespan=lifegen.getLifespans(num_samples);
        ISetSegments segs(lifespan);
        greedy.update(s,segs);
        std::cout<<temp<<" ";
        double greedy_basic=greedy.getResult();
        std::cout<<"greedy_basic "<<greedy_basic<<" ";
        greedy.next();

        basic.update(s,segs);

        double basic_pdg=basic.getResult();
        std::cout<<"basic_pdg "<<basic_pdg<<std::endl;
        basic.next();
        out<<temp<<'\t'<<greedy_basic<<'\t'<<basic_pdg<<std::endl;
        temp++;
    }

    return 0;
}