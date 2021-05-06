//
// Created by weiwei on 2021/5/5.
//

#ifndef INFLUENCERS_TRACKING_BASIC_PDG_H
#define INFLUENCERS_TRACKING_BASIC_PDG_H

#include "sieve_pag.h"
#include "bernoulli_segment.h"

class BasicPAG{
private:
    int L_,cur_=0;
    std::vector<SievePAG*> sieve_ptrs_;
public:
    BasicPAG(const int L,const int budget,const double eps,const int num_samples):L_(L){
        sieve_ptrs_.resize(L);
        for(int l=0;l<L_;l++){
            sieve_ptrs_[l]=new SievePAG(num_samples,budget,eps);
        }
    }

    virtual ~BasicPAG(){
        for(int l=0;l<L_;l++){
            delete sieve_ptrs_[l];
        }
    }

    void update(SocialAc s,const BernoulliSegments& segs){
        for (auto& seg : segs.segments_)
            for (int i = seg.start_; i < seg.end_; ++i)
                sieve_ptrs_[(i + cur_) % L_]->update(s, seg.bs_);
    }

//    std::pair<int, double> getResult() const {
//        return sieve_ptrs_[cur_]->getResult();
//    }

    double getResult() const {
        return sieve_ptrs_[cur_]->getResult();
    }
    void next() {
        sieve_ptrs_[cur_]->clear(true);
        for(int i=0;i<L_;i++){
            sieve_ptrs_[i]->clear();
        }
        cur_ = (cur_ + 1) % L_;
    }

};



#endif //INFLUENCERS_TRACKING_BASIC_PDG_H
