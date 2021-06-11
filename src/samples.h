//
// Created by weiwei on 2021/6/9.
//

#ifndef INFLUENCERS_TRACKING_SAMPLES_H
#define INFLUENCERS_TRACKING_SAMPLES_H

#include "stdafx.h"
#include "social_influence.h"

std::string into2bin(int n,int count){
    std::string str="";
    for(int i=count-1;i>=0;i--){
        auto temp=(n>>i)&1;
        str+=std::to_string(temp);
    }
    return str;
}

double get_prob(double lambda,int time,int ta){
    return exp(-lambda*(time-ta));
}

double cal_nrmse(std::vector<double> &s,double expect){
    double sum=0;
    for(auto item:s){
        sum+= pow(item-expect,2);
    }
    double nrmse= sqrt(sum/s.size())/expect;
    return nrmse;
}

double get_expect(const double lambda, int k, SocialInfluence &s_sin, int time, std::map<int,int> u_t){
    double expect=0;
    for(int i=1;i<pow(2, k); i++){
        std::string t= into2bin(i, k);
        double temp_prob=1;
        auto iter=u_t.begin();
        std::vector<int> u;
        for(int j=0; j < k; j++){
            int ex;
            if(t[j]=='1'){
                u.push_back(iter->first);
                ex=1;
            }else{
                ex=0;
            }
            temp_prob=temp_prob*pow(get_prob(lambda,time,iter->second),ex)
                      *pow(1-get_prob(lambda,time,iter->second), 1-ex);
            iter++;
        }
        double reward=s_sin.getReward(u);
        expect+=temp_prob*reward;
    }
    return expect;
}

#endif //INFLUENCERS_TRACKING_SAMPLES_H
