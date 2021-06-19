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
    std::vector<int> affected_;

public:
    ObjMgr() {}
    ObjMgr(const int n) { fun_vec_.resize(n); }

    // add a social action and its I-set to the objective function
    void update(const Action& a, const ISet& iset);

    // Ft(u)
    double getVal(const int u) const;

    // Ft(S)
    double getVal(const std::vector<int>& S) const;

    // Delta(u|S)
    double getGain(const int u, const std::vector<int>& S) const;

    // Va
    std::vector<int> getVa() const { return affected_; }

    int getOracleCalls() const;

}; /* ObjMgr */

#endif /* __OBJ_MGR_H__ */
