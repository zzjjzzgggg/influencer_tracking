//
// Created by weiwei on 2021/6/2.
//

#ifndef INFLUENCERS_TRACKING_BASIC_IT_H
#define INFLUENCERS_TRACKING_BASIC_IT_H

#include "stdafx.h"
#include "sieve_pait.h"
#include "iset_segment.h"

class BasicIT {
private:
    int L_, cur_ = 0;
    std::vector<SievePAIT*> sieve_ptrs_;

public:
    BasicIT(const int L, const int budget, const double eps,
            const int num_samples)
        : L_(L) {
        sieve_ptrs_.resize(L);
        for (int l = 0; l < L_; l++) {
            sieve_ptrs_[l] = new SievePAIT(num_samples, budget, eps);
        }
    }

    virtual ~BasicIT() {
        for (int l = 0; l < L_; l++) {
            delete sieve_ptrs_[l];
        }
    }

    void update(const SocialAc& a, const ISetSegments& segs) {
        for (auto& seg : segs.segments_)
            for (int i = seg.start_; i < seg.end_; ++i)
                sieve_ptrs_[(i + cur_) % L_]->update(a, seg.is_);
    }

    double getResult() const { return sieve_ptrs_[cur_]->getResult(); }

    void next() {
        sieve_ptrs_[cur_]->clear(true);
        for (int i = 0; i < L_; i++) {
            sieve_ptrs_[i]->clear();
        }
        cur_ = (cur_ + 1) % L_;
    }

    int statOracleCalls() {
        int oracle_calls = 0;
        for (int i = 0; i < L_; i++) {
            oracle_calls += sieve_ptrs_[i]->getOracleCalls();
        }
        return oracle_calls;
    }
};

#endif  // INFLUENCERS_TRACKING_BASIC_IT_H
