//
// Created by weiwei on 2021/5/10.
//

#ifndef INFLUENCERS_TRACKING_GREEDY_BASIC_H
#define INFLUENCERS_TRACKING_GREEDY_BASIC_H

#include "stdafx.h"
#include "greedy_pait.h"
#include "iset_segment.h"

class GreedyBasic{
private:
    int L_,cur_=0;
    std::vector<GreedyAlg*> greedy_ptrs_;
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

GreedyBasic::GreedyBasic(const int L,const int budget,const int num_samples){
    L_=L;
    greedy_ptrs_.resize(L);
    for(int l=0;l<L_;l++){
        greedy_ptrs_[l]=new GreedyAlg(num_samples,budget);
    }
}

void GreedyBasic::update(SocialAc s,const ISetSegments& segs){
    for(auto& seg:segs.segments_){
        for(int i=seg.start_;i<seg.end_;i++){
            greedy_ptrs_[(i+cur_)%L_]->update(s,seg.is_);
        }
    }
}

void GreedyBasic::next(){
    greedy_ptrs_[cur_]->clear();
    for(int i=0;i<L_;i++){
        greedy_ptrs_[i]->clear();
    }
    cur_=(cur_+1)%L_;
}

double GreedyBasic::getResult()const {
    return greedy_ptrs_[cur_]->getResult();
}


#endif //INFLUENCERS_TRACKING_GREEDY_BASIC_H
