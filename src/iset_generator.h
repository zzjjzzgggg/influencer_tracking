//
// Created by weiwei on 2021/4/18.
//

#ifndef INFLUENCERS_TRACKING_ISET_GENERATOR_H
#define INFLUENCERS_TRACKING_ISET_GENERATOR_H



#include "stdafx.h"

//get I-set
class ISetGenerator {
private:
    const int n_;
    const double p_;
    rngutils::default_rng rng_;
public:
    ISetGenerator(const int n, const double p) : n_(n), p_(p) {}

    ISet getISet() {
        ISet is;
        for (int i = 0; i < n_; i++) {
            if (rng_.uniform() <= p_) is.push_back(i);
        }
        return is;
    }
};

#endif //INFLUENCERS_TRACKING_ISET_GENERATOR_H