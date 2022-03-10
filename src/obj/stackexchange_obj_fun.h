/**
 * Copyright (C) by J.Z. 2021-06-19 11:33
 * Distributed under terms of the MIT license.
 */

#ifndef __STACKEXCHANGE_OBJ_FUN_H__
#define __STACKEXCHANGE_OBJ_FUN_H__

#include "obj_fun.h"

/**
 * Objective function for StackExchange and Reddit dataset. Not sure whether it
 * is suitable for other datasets.
 */
class StackExObjFun : public ObjFun {
public:
    mutable int oracle_calls_ = 0;
    std::unordered_set<int> affected_;
    std::unordered_map<int, std::unordered_set<int>> user_sigma_;

public:
    StackExObjFun() {}
    StackExObjFun(const StackExObjFun &o):affected_(o.affected_),
                            user_sigma_(o.user_sigma_){oracle_calls_=0;}
    void add(const Action& a) {
        if (user_sigma_.find(a.u) == user_sigma_.end()) {
            // if user u is a new user
            affected_.insert(a.u);
            user_sigma_[a.u].insert(a.u);
        } else if (user_sigma_[a.u].find(a.v) == user_sigma_[a.u].end()) {
            // else if v is not included in user u's influence scope
            affected_.insert(a.u);
            user_sigma_[a.u].insert(a.v);
        }
        if (user_sigma_.find(a.v) == user_sigma_.end()) {
            // if user v is a new user
            affected_.insert(a.v);
            user_sigma_[a.v].insert(a.v);
        }
    }

    // If deep = true, clean every thing; this is used when reseting SievePAIT
    // is need in BasicIT.
    void clear(const bool deep = false) {
        oracle_calls_ = 0;
        affected_.clear();
        if (deep) user_sigma_.clear();
    }

    // Get the reward of a single user.
    double getVal(const int u) const {
        if (user_sigma_.find(u) == user_sigma_.end()) return 0;
        return user_sigma_.at(u).size();
    }

    // Get the reward of a user set.
    double getVal(const std::vector<int>& S) const {
        std::unordered_set<int> scope;
        for (int u : S) {
            if (user_sigma_.find(u) != user_sigma_.end()) {
                const auto& set = user_sigma_.at(u);
                scope.insert(set.begin(), set.end());
            }
        }
        return scope.size();
    }

    // Get the gain of adding a user v to a user set S.
    double getGain(const int v, const std::vector<int>& S) const {
        if (user_sigma_.find(v) == user_sigma_.end()) return 0;
        ++oracle_calls_;

        std::unordered_set<int> scope;

        // firstly, calculate scope = sigma(S)
        for (int u : S) {
            if (user_sigma_.find(u) != user_sigma_.end()) {
                const auto& set = user_sigma_.at(u);
                scope.insert(set.begin(), set.end());
            }
        }
        int val = scope.size();

        // secondly, calculate scope = sigma(S U {v})
        const auto& set = user_sigma_.at(v);
        scope.insert(set.begin(), set.end());

        // finally, calculate gain
        return scope.size() - val;
    }

}; /* StackExObjFun */

#endif /* __STACKEXCHANGE_OBJ_FUN_H__ */
