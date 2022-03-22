//
// Created by zhangwei on 2021/4/20.
//

#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "stdafx.h"

/**
 * The candidate items corresponding to a threshold.
 */
class Candidate {
public:
    std::unordered_set<int> S_;  // S_theta
public:
    Candidate() {}

    inline bool isMember(const int e) const { return S_.find(e) != S_.end(); }

    inline int size() const { return S_.size(); }

    void clear() { S_.clear(); }
    void insert(const int e) { S_.insert(e); }

    std::vector<int> getMembers() const {
        return std::vector<int>(S_.begin(), S_.end());
    }

}; /* Candidate */

#endif  // CANDIDATE_H
