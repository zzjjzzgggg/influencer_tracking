//
// Created by weiwei on 2021/5/29.
//

#ifndef INFLUENCERS_TRACKING_SOCIAL_INFLUENCE_H
#define INFLUENCERS_TRACKING_SOCIAL_INFLUENCE_H

#include "stdafx.h"

class SocialInfluence{
public:
    mutable int oracle_calls_ = 0;
    std::vector<int> users_;
    std::unordered_set<int> affected_nodes_;
    std::map<int,std::set<int>> user_sigma_;
public:
    SocialInfluence(){}
    SocialInfluence(const SocialInfluence& o){
        oracle_calls_=o.oracle_calls_;
        users_=o.users_;
        affected_nodes_=o.affected_nodes_;
        user_sigma_=o.user_sigma_;
    }

    SocialInfluence& operator=(const SocialInfluence& o){
        oracle_calls_=o.oracle_calls_;
        users_=o.users_;
        affected_nodes_=o.affected_nodes_;
        user_sigma_=o.user_sigma_;
        return *this;
    }

    inline bool exists(const int u){
        bool is= std::find(users_.begin(), users_.end(), u) != users_.end();
        return is;
    }

    void addSocialAction(const int u,const int v,const int c);

    double getReward(const int u);

    double getReward(const std::vector<int> &S);

    double getGain(const int u,const std::vector<int>& S);

    std::vector<int> getAffectedNodes(){
        return std::vector<int>(affected_nodes_.begin(),affected_nodes_.end());
    }

    void clear(const bool deep= false){
        oracle_calls_=0;
        affected_nodes_.clear();
        if(deep){
            users_.clear();
            user_sigma_.clear();
        }
    }

    std::vector<int> getNodes(){
        return std::vector<int>(users_.begin(),users_.end());
    }

    int getOracleCalls()const {return oracle_calls_;}
};



void SocialInfluence::addSocialAction(const int u, const int v, const int c) {
    //map: automatic deduplication
    user_sigma_[u].insert(u);
    user_sigma_[u].insert(v);
    user_sigma_[v].insert(v);
    affected_nodes_.insert(u);
    affected_nodes_.insert(v);
    if(!exists(u))
        users_.push_back(u);
    if(!exists(v))
        users_.push_back(v);
}

double SocialInfluence::getReward(const int u) {
    return user_sigma_[u].size();
}

double SocialInfluence::getReward(const std::vector<int> &S){
    double gain=0;
    for(auto &item:S){
        if(exists(item))
            gain+=user_sigma_[item].size();
    }
    return gain;
}

double SocialInfluence::getGain(const int u, const std::vector<int> &S) {
    //u not in subgraphs
    oracle_calls_++;

    if(user_sigma_.find(u) == user_sigma_.end())
    {
        return 0;
    }

    double rwd_S=0;
    for(auto &item:S){
        rwd_S+=user_sigma_[item].size();
    }

    double gain_sum= rwd_S + user_sigma_[u].size();
    double gain=gain_sum-rwd_S;
    return gain;
}

#endif //INFLUENCERS_TRACKING_SOCIAL_INFLUENCE_H
