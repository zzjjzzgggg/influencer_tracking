//
// Created by weiwei on 2021/5/10.
//

#ifndef INFLUENCERS_TRACKING_GREEDY_BASIC_H
#define INFLUENCERS_TRACKING_GREEDY_BASIC_H

#include "stdafx.h"
#include "greedy_pait.h"
#include "iset_segment.h"

/**
 * GreedyBasic
 */
template <typename Fun>
class GreedyBasic{
private:
    int L_,cur_=0;
    std::vector<GreedyAlg<Fun>*> greedy_ptrs_;
public:
    double getResult(std::vector<int> nodes);

    virtual ~GreedyBasic(){
        for(int l=0;l<L_;l++){
            delete greedy_ptrs_[l];
        }
    }

    int statOracleCalls(){
        int oracle_calls=0;
        for(int i=0;i<L_;i++){
            oracle_calls+=greedy_ptrs_[i]->getOracleCalls();
        }
        return oracle_calls;
    }

    GreedyBasic(const int L,const int budget,const int num_samples){
        L_=L;
        greedy_ptrs_.resize(L);
        for(int l=0;l<L_;l++){
            greedy_ptrs_[l]=new GreedyAlg<Fun>(num_samples,budget);
        }
    }

    void update(const Action &a,const ISetSegments& segs){
        for(auto& seg:segs.segments_){
            for(int i=seg.start_;i<seg.end_;i++){
                greedy_ptrs_[(i+cur_)%L_]->update(a,seg.is_);
            }
        }
    }

    void next(){
        greedy_ptrs_[cur_]->clear(true);
        for(int i=0;i<L_;i++){
            greedy_ptrs_[i]->clear();
        }
        cur_=(cur_+1)%L_;
    }

    double getResult()const {
        return greedy_ptrs_[cur_]->getResult();
    }
};


#endif //INFLUENCERS_TRACKING_GREEDY_BASIC_H
