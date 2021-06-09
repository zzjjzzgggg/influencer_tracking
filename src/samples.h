//
// Created by weiwei on 2021/6/9.
//

#ifndef INFLUENCERS_TRACKING_SAMPLES_H
#define INFLUENCERS_TRACKING_SAMPLES_H

#include "stdafx.h"

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

double get_expect(){
    double test_pa=0.9;
    double expect=0;
    for(int i=1;i<pow(2,num_sample);i++){
        std::string t= into2bin(i,num_sample);
        double temp_prob=1;
        for(int j=0;j<num_sample;j++){
            int ex;
            std::vector<int> u;
            if(t[j]=='1'){
                u.push_back()
                ex=1;
            }else{
                ex=0;
            }

            temp_prob=temp_prob*pow(test_pa),ex)*
                      pow((1-test_pa), 1-ex);
        }
        expect+=temp_prob*;
    }
    return expect;
}



#endif //INFLUENCERS_TRACKING_SAMPLES_H
