/**
 * Copyright (C) by J.Z. 2021-06-19 11:19
 * Distributed under terms of the MIT license.
 */

#ifndef __OBJ_MGR_H__
#define __OBJ_MGR_H__

#include "stdafx.h"

/**
 * ObjMgr maintains a collection of ObjFun objects and each ObjFun object is an
 * objective function built on a data stream sample. These ObjFun objects
 * together define the surrogate objective function Ft in the paper.
 */
template <typename Fun>
class ObjMgr {
private:
    std::vector<Fun> fun_vec_;

public:
    ObjMgr() {}
    ObjMgr(const int n) { fun_vec_.resize(n); }
    ObjMgr(const ObjMgr &o):fun_vec_(o.fun_vec_){}
    // add a social action and its I-set to the objective function
    void update(const Action& a, const ISet& iset) {
        for (int i : iset) fun_vec_[i].add(a);
    }

    // Ft(u)
    double getVal(const int u) const {
        double val = 0;
        for (auto& fun : fun_vec_) val += fun.getVal(u);
        return val / fun_vec_.size();
    }

    // Ft(S)
    double getVal(const std::vector<int>& S) const {
        double val = 0;
        for (auto &fun : fun_vec_) val += fun.getVal(S);
        return val / fun_vec_.size();
    }

    // Delta(u|S)
    double getGain(const int v, const std::vector<int>& S) const {
        double gain = 0;
        for (auto& fun : fun_vec_) gain += fun.getGain(v, S);
        return gain / fun_vec_.size();
    }

    // Va
    std::vector<int> getVa() const {
        std::unordered_set<int> set;
        for (auto& fun : fun_vec_)
            set.insert(fun.affected_.begin(), fun.affected_.end());
        return std::vector<int>(set.begin(), set.end());
    }

    int getOracleCalls() const {
        int calls = 0;
        for (auto& fun : fun_vec_) calls += fun.oracle_calls_;
        return calls;
    }

    std::unordered_set<int> getUsers()const{
        std::unordered_set<int> users;
        for(auto& fun:fun_vec_){
            for(auto& u:fun.user_sigma_)
                users.insert(u.first);
        }
        return std::unordered_set<int>(users.begin(),users.end());
    }
    void clear(const bool deep= false){
        for (auto& fun : fun_vec_){
            fun.clear(deep);
        }
    }

}; /* ObjMgr */

#endif /* __OBJ_MGR_H__ */
