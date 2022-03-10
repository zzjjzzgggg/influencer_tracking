/**
 * Copyright (C) by J.Z. 2021-06-19 20:58
 * Distributed under terms of the MIT license.
 */

#ifndef __CHECKIN_OBJ_FUN_H__
#define __CHECKIN_OBJ_FUN_H__

#include "obj_fun.h"

/**
 * Objective function for the check-in dataset.
 */
class CheckinObjFun : public ObjFun {
public:
    mutable int oracle_calls_ = 0;
    std::unordered_set<int> affected_;
    std::unordered_map<int, std::unordered_set<int>> loc_sigma_;

public:
    CheckinObjFun() {}
    CheckinObjFun(const CheckinObjFun& o)
        : affected_(o.affected_), oracle_calls_(0), loc_sigma_(o.loc_sigma_) {}

    // a.u: location, a.v: user
    void add(const Action& a) {
        if (loc_sigma_.find(a.u) == loc_sigma_.end()) {
            // if location u is a new location
            affected_.insert(a.u);
            loc_sigma_[a.u].insert(a.v);
        } else if (loc_sigma_[a.u].find(a.v) == loc_sigma_[a.u].end()) {
            // else if v is not included in location u's influence scope
            affected_.insert(a.u);
            loc_sigma_[a.u].insert(a.v);
        }
    }

    // If deep = true, clean every thing; this is used when reseting SievePAIT
    // is need in BasicIT
    void clear(const bool deep = false) {
        oracle_calls_ = 0;
        affected_.clear();
        if (deep) loc_sigma_.clear();
    }

    // Get the reward of a location u.
    double getVal(const int u) const {
        if (loc_sigma_.find(u) == loc_sigma_.end()) return 0;
        return loc_sigma_.at(u).size();
    }

    // Get the reward of a location set.
    double getVal(const std::vector<int>& S) const {
        std::unordered_set<int> scope;
        for (int u : S) {
            if (loc_sigma_.find(u) != loc_sigma_.end()) {
                const auto& set = loc_sigma_.at(u);
                scope.insert(set.begin(), set.end());
            }
        }
        return scope.size();
    }

    // Get the gain of adding a location v to a location set S.
    double getGain(const int v, const std::vector<int>& S) const {
        if (loc_sigma_.find(v) == loc_sigma_.end()) return 0;
        ++oracle_calls_;

        std::unordered_set<int> scope;

        // firstly, calculate scope = sigma(S)
        for (int u : S) {
            if (loc_sigma_.find(u) != loc_sigma_.end()) {
                const auto& set = loc_sigma_.at(u);
                scope.insert(set.begin(), set.end());
            }
        }

        int val = scope.size();

        // secondly, calculate scope = sigma(S U {v})
        const auto& set = loc_sigma_.at(v);
        scope.insert(set.begin(), set.end());

        // finally, calculate gain
        return scope.size() - val;
    }

}; /* CheckinObjFun */

#endif /* __CHECKIN_OBJ_FUN_H__ */
