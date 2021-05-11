//
// Created by weiwei on 2021/5/10.
//

#ifndef INFLUENCERS_TRACKING_GREEDY_BASIC_H
#define INFLUENCERS_TRACKING_GREEDY_BASIC_H

#include "stdafx.h"
#include "greedy_alg.h"
#include "iset_segment.h"

template<class InputMgr>
class GreedyBasic{
private:
    int L_,cur_=0;
    std::vector<GreedyAlg<InputMgr>*> greedy_ptrs_;
public:
    GreedyBasic(const int L,const int budget,const int num_samples);
    double getResult(std::vector<int> nodes);

    virtual ~GreedyBasic(){
        for(int l=0;l<L_;l++){
            delete greedy_ptrs_[l];
        }
    }

    void update(SocialAc s,const ISetSegments& segs);

    double getResult()const;

    void next();
};

template<class InputMgr>
GreedyBasic<InputMgr>::GreedyBasic(const int L,const int budget,const int num_samples){
    L_=L;
    greedy_ptrs_.resize(L);
    for(int l=0;l<L_;l++){
        greedy_ptrs_[l]=new GreedyAlg<InputMgr>(num_samples,budget);
    }
}

template<class InputMgr>
void GreedyBasic<InputMgr>::update(SocialAc s,const ISetSegments& segs){
    for(auto& seg:segs.segments_){
        for(int i=seg.start_;i<seg.end_;i++){
            greedy_ptrs_[(i+cur_)%L_]->update(s,seg.is_);
        }
    }
}

template<class InputMgr>
void GreedyBasic<InputMgr>::next(){
    greedy_ptrs_[cur_]->clear(true);
    for(int i=0;i<L_;i++){
        greedy_ptrs_[i]->clear();
    }
    cur_=(cur_+1)%L_;
}

template<class InputMgr>
double GreedyBasic<InputMgr>::getResult()const {
    return greedy_ptrs_[cur_]->getResult();
}


#endif //INFLUENCERS_TRACKING_GREEDY_BASIC_H
