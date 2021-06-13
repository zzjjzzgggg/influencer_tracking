//
// Created by weiwei on 2021/6/9.
//

#ifndef INFLUENCERS_TRACKING_SAMPLES_H
#define INFLUENCERS_TRACKING_SAMPLES_H

#include "stdafx.h"

double get_prob(double lambda,int time,int ta){
    return exp(-lambda*(time-ta));
}

#endif //INFLUENCERS_TRACKING_SAMPLES_H
