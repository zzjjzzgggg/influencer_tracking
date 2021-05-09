//
// Created by weiwei on 2021/5/8.
//

#include "hist_pdg.h"
#include "basic_pdg.h"
#include "ISet_segment.h"
#include "lifespan_generator.h"
#include "dyn_dgraph_mgr_v2.h"
//#include "dyn_bgraph_mgr.h"
int main(){
    int L=40;
    int budget=10;
    double eps=0.2;
    int num_samples=10;
//    double p=0.6;

    double lmd=0.01;
    LifespanGenerator lifegen(L,1-exp(-lmd));
    std::string filename="output2.txt";
    std::ifstream data(filename);
    std::string oneline;
    int x=0;
    SocialAcs ss;
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
        if(x==1000)
            break;
    }

    HistPDG hist(num_samples,budget,eps);
    BasicPAG<DynDGraphMgr> basic(L,budget,eps,num_samples);
    ss.push_back({{1,2},1619009223});
    ss.push_back({{1,2},1619009223});
    ss.push_back({{1,3},1619009223});
    ss.push_back({{3,4},1619009223});
    ss.push_back({{4,5},1619009223});
    int i=1;

    int temp=1;
    std::string outfile="hist.txt";
    std::ofstream out(outfile);
    for(auto &s:sc){
        std::vector<int> lifespan=lifegen.getLifespans(num_samples);
        ISetSegments segs(lifespan);
//        for(auto seg:segs.segments_){
//            std::cout<<"start"<<seg.start_<<" "<<seg.end_<<std::endl;
//        }
        hist.feed(s,segs);
        std::cout<<i++<<" ";
        double hist_mx=hist.getResult();
        std::cout<<"hist"<<hist_mx<<"   ";
//        hist.reduce();

        hist.next();

        basic.update(s,segs);

        double basic_mx=basic.getResult();
        std::cout<<"basic"<<basic_mx<<std::endl;
        basic.next();

        out<<temp<<'\t'<<basic_mx<<'\t'<<hist_mx<<std::endl;
        temp++;
    }

    return 0;
}
