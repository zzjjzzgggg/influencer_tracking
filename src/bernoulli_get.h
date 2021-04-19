//
// Created by weiwei on 2021/4/18.
//

#ifndef SIEVEPAG_BERNOULLI_GET_H
#define SIEVEPAG_BERNOULLI_GET_H

#endif //SIEVEPAG_BERNOULLI_GET_H

#include "stdafx.h"

//get I-set
class BernoulliSetGenerator {
private:
    const int n_;
    const double p_;
    rngutils::default_rng rng_;
public:
    BernoulliSetGenerator(const int n, const double p) : n_(n), p_(p) {}

    BernoulliSet getBernoulliSet() {
        BernoulliSet bs;
        for (int i = 0; i < n_; i++) {
            if (rng_.uniform() <= p_) bs.push_back(i);
        }
        return bs;
    }
};