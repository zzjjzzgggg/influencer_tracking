//
// Created by weiwei on 2021/4/20.
//

#ifndef TEST_CMAKE_CANDIDATE_H
#define TEST_CMAKE_CANDIDATE_H

#include "stdafx.h"
//#include "obj_fun.h"
/**
 * The candidate items corresponding to a threshold.
 */

class Candidate {
public:
    std::unordered_set<int> S_;            // S_theta
public:
    Candidate() {}

    inline bool isMember(const int e) const { return S_.find(e) != S_.end(); }

    inline int size() const { return S_.size(); }

    void clear() {
        S_.clear();
    }
    void insert(const int e){
        S_.insert(e);
    }

    std::vector<int> getMembers() const {
        return std::vector<int>(S_.begin(), S_.end());
    }


}; /* Candidate */

#endif //TEST_CMAKE_CANDIDATE_H
