//
// Created by weiwei on 2021/5/30.
//

#ifndef INFLUENCERS_TRACKING_GREEDY_PAIT_H
#define INFLUENCERS_TRACKING_GREEDY_PAIT_H

#include "stdafx.h"
#include "obj_mgr.h"
#include "candidate.h"

template<typename Fun>
class GreedyAlg{
public:
    int num_samples_,budget_;     //n and k
    ObjMgr<Fun> obj_mgr_;       // maintains n objective functions
    std::unordered_set<int> users_;

public:
    GreedyAlg(const int num_samples,const int budget):num_samples_(num_samples), budget_(budget),obj_mgr_(num_samples_){
    }
    double getResult();
    void clear(const bool deep=false){
        obj_mgr_.clear(deep);
        if(deep){
            users_.clear();
        }
    }
    void update(const Action &a, const ISet &iset) {
        //add social action
        obj_mgr_.update(a, iset);
        if(users_.find(a.u)==users_.end()){
            users_.insert(a.u);
        }
        if(users_.find(a.v)==users_.end()){
            users_.insert(a.v);
        }
    }

    int getOracleCalls() const { return obj_mgr_.getOracleCalls(); }
};

template <typename Fun>
double GreedyAlg<Fun>::getResult(){

    //get nodes
    std::unordered_set<int> users=users_;

    std::vector<int> S;
    int mx_gain_u;
    for(int i=0;i<budget_;i++){
        double gain_mx=-100;
        for(auto &u:users){
            double gain=obj_mgr_.getGain(u,S);
            if(gain>gain_mx){
                gain_mx=gain;
                mx_gain_u=u;
            }
        }
        S.push_back(mx_gain_u);
        users.erase(mx_gain_u);
        if(users.empty()){
            break;
        }
    }
    double rwd_mx=obj_mgr_.getVal(S);
    return  rwd_mx;
}


#endif //INFLUENCERS_TRACKING_GREEDY_PAIT_H
