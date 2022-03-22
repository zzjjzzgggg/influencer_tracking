//
// Created by zhangwei on 2021/4/18.
//

#ifndef INFLUENCERS_TRACKING_ISET_GENERATOR_H
#define INFLUENCERS_TRACKING_ISET_GENERATOR_H

#include "stdafx.h"

// get I-set
class ISetGenerator {
private:
    int n_;
    double p_;
    rngutils::default_rng rng_;

public:
    ISetGenerator() {}
    ISetGenerator(const int n, const double p) : n_(n), p_(p) {}

    ISet getISet() {
        ISet is;
        for (int i = 0; i < n_; i++) {
            if (rng_.uniform() <= p_) is.push_back(i);
        }
        return is;
    }

    ISet getISet(const int n, const double p) {
        ISet is;
        for (int i = 0; i < n; i++) {
            if (rng_.uniform() <= p) is.push_back(i);
        }
        return is;
    }
};

#endif  // INFLUENCERS_TRACKING_ISET_GENERATOR_H
