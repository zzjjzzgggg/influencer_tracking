//
// Created by weiwei on 2021/5/5.
//
#include "basic_pdg.h"
#include "bernoulli_segment.h"
#include "lifespan_generator.h"

int main(){
    int L=4;
    int budget=10;
    double eps=0.1;
    int num_samples=10;
    double p=0.6;

    double lmd=0.01;
    BasicPAG basic(L,budget,eps,num_samples);

    LifespanGenerator lifegen(L,1-exp(-lmd));

    SocialAcs ss;
    ss.push_back({{1,2},1619009223});
    ss.push_back({{1,3},1619009223});

    for(auto &s:ss){
        std::vector<int> lifespan=lifegen.getLifespans(num_samples);
        BernoulliSegments segs(lifespan);
//        pag.feed(s,bs);
        basic.update(s,segs);
//        std::cout<<temp<<" ";

        double pag_mx=basic.getResult();
        std::cout<<pag_mx<<std::endl;
        basic.next();
//        pag.clear();
////        temp++;
    }


    return 0;
}

