//
// Created by weiwei on 2021/5/5.
//

#include "lifespan_generator.h"

int main(){
    int n=10;
    int L=500;
    int T=10;
    double lmd=0.01;
    double p;
    LifespanGenerator lifegen(L,1-exp(-lmd));

    int t=0;
    while(t++<T){
        auto lifespan=lifegen.getLifespans(n);
        for(auto item:lifespan){
            std::cout<<item<<" ";
        }
        std::cout<<std::endl;
    }
    return 0;
}