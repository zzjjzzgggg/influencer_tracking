//
// Created by weiwei on 2021/5/5.
//

#include "lifespan_generator.h"
#include "iset_segment.h"

int main(){
    int n=10;
    int L=500;
    int T=10;
    double lmd=0.01;
    LifespanGenerator lifegen(L,1-exp(-lmd));
    std::vector<int> lifespan=lifegen.getLifespans(10);
    ISetSegments segs(lifespan);
    for(auto item:segs.segments_){
        std::cout<<item.start_<<" "<<item.end_<<std::endl;
        for(auto i:item.is_){
            std::cout<<i<<" ";
        }
        std::cout<<std::endl;
    }
    for(auto item:lifespan){
        std::cout<<item<<std::endl;
    }
//    int t=0;
//    while(t++<T){
//        auto lifespan=lifegen.getLifespans(n);
//        for(auto item:lifespan){
//            std::cout<<item<<" ";
//        }
//        std::cout<<std::endl;
//    }
    return 0;
}