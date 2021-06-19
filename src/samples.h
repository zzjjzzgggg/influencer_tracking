//
// Created by weiwei on 2021/6/9.
//

#ifndef INFLUENCERS_TRACKING_SAMPLES_H
#define INFLUENCERS_TRACKING_SAMPLES_H

#include "stdafx.h"

double get_prob(double lambda,int time,int ta){
    return exp(-lambda*(time-ta));
}

double get_mse(std::vector<double> fts_set,double fts_true){
    double mse_sum=0;
    for(auto item:fts_set){
        mse_sum+= pow(item-fts_true,2);
    }
    return mse_sum/fts_set.size();
}

#endif //INFLUENCERS_TRACKING_SAMPLES_H
