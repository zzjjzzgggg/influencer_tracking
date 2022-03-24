//
// Created by zhangwei on 2021/6/2.
//

#ifndef INFLUENCERS_TRACKING_BASIC_IT_H
#define INFLUENCERS_TRACKING_BASIC_IT_H

#include "stdafx.h"
#include "sieve_pait.h"
#include "iset_segment.h"

/**
 * BasicIT
 */
template <typename Fun,typename InputData>
class BasicIT {
private:
    int L_, cur_ = 0;
    std::vector<SievePAIT<Fun,InputData>*> sieve_ptrs_;

public:
    BasicIT(const int L, const int budget, const double eps,
            const int num_samples)
        : L_(L) {
        sieve_ptrs_.resize(L);
        for (int l = 0; l < L_; l++) {
            sieve_ptrs_[l] = new SievePAIT<Fun,InputData>(num_samples, budget, eps);
        }
    }

    virtual ~BasicIT() {
        for (int l = 0; l < L_; l++) {
            delete sieve_ptrs_[l];
        }
    }

    void update(const InputData &a, const ISetSegments& segs) {
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

    /**
     * Get the cumulative number of oracle calls.
     */
    int statOracleCalls() {
        int oracle_calls = 0;
        for (int i = 0; i < L_; i++) {
            oracle_calls += sieve_ptrs_[i]->getOracleCalls();
        }
        return oracle_calls;
    }
};

#endif  // INFLUENCERS_TRACKING_BASIC_IT_H
