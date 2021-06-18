//
// Created by weiwei on 2021/5/29.
//

#ifndef INFLUENCERS_TRACKING_SOCIAL_INFLUENCE_H
#define INFLUENCERS_TRACKING_SOCIAL_INFLUENCE_H

#include "stdafx.h"

class SocialInfluence {
public:
    mutable int oracle_calls_ = 0;
    std::vector<int> users;
    std::unordered_set<int> affected_nodes_;
    std::map<int, std::set<int>> user_sigma_;

public:
    SocialInfluence() {}
    SocialInfluence(const SocialInfluence &o) {
        oracle_calls_ = o.oracle_calls_;
        users = o.users;
        affected_nodes_ = o.affected_nodes_;
        user_sigma_ = o.user_sigma_;
    }

    SocialInfluence &operator=(const SocialInfluence &o) {
        oracle_calls_ = o.oracle_calls_;
        users = o.users;
        affected_nodes_ = o.affected_nodes_;
        user_sigma_ = o.user_sigma_;
        return *this;
    }

    inline bool exists(const int u) {
        return std::find(users.begin(), users.end(), u) != users.end();
    }

    void addSocialAction(const int u, const int v, const int c);

    double getReward(const int u);

    double getReward(const std::vector<int> &S);

    double getGain(const int u, const std::vector<int> &S);

    std::vector<int> getAffectedNodes() {
        return std::vector<int>(affected_nodes_.begin(), affected_nodes_.end());
    }

    void clear(const bool deep = false) {
        oracle_calls_ = 0;
        affected_nodes_.clear();
        if (deep) {
            user_sigma_.clear();
        }
    }

    std::vector<int> getNodes() {
        std::vector<int> nodes;
        for (auto iter = user_sigma_.begin(); iter != user_sigma_.end(); iter++) {
            nodes.push_back(iter->first);
        }
        return nodes;
    }

    int getOracleCalls() const { return oracle_calls_; }
};

void SocialInfluence::addSocialAction(const int u, const int v, const int c) {
    // map: automatic deduplication
    user_sigma_[u].insert(u);
    user_sigma_[u].insert(v);
    user_sigma_[v].insert(v);
    affected_nodes_.insert(u);
    affected_nodes_.insert(v);
    if (!exists(u)) users.push_back(u);
    if (!exists(v)) users.push_back(v);
}

double SocialInfluence::getReward(const int u) { return user_sigma_[u].size(); }

double SocialInfluence::getReward(const std::vector<int> &S) {
    double gain = 0;
    for (auto &item : S) {
        if (exists(item)) gain += user_sigma_[item].size();
    }
    return gain;
}

double SocialInfluence::getGain(const int u, const std::vector<int> &S) {
    // u not in subgraphs
    oracle_calls_++;

    if (user_sigma_.find(u) == user_sigma_.end()) {
        return 0;
    }

    double rwd_S = 0;
    for (auto &item : S) {
        rwd_S += user_sigma_[item].size();
    }

    double gain_sum = rwd_S + user_sigma_[u].size();
    double gain = gain_sum - rwd_S;
    return gain;
}

#endif  // INFLUENCERS_TRACKING_SOCIAL_INFLUENCE_H
