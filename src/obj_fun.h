/**
 * Copyright (C) by J.Z. 2019-03-25 16:37
 * Distributed under terms of the MIT license.
 */

#ifndef __OBJ_FUN_H__
#define __OBJ_FUN_H__

#include "stdafx.h"

class ObjFun {
public:
    virtual ~ObjFun() {}

    virtual double getVal(const int) const = 0;
    virtual double getVal(const std::vector<int>&) const = 0;
    virtual double getGain(const int, const std::vector<int>&) const = 0;

}; /* ObjFun */

/**
 * ObjMgr maintains a collection of ObjFun objects and each ObjFun object is an
 * objective function built on a data stream sample. These ObjFun objects
 * together define the surrogate objective function Ft in the paper.
 */
class ObjMgr {
private:
    std::vector<ObjFun*> obj_vec_;

public:
    ObjMgr(const int n) { obj_vec_.reserve(n); }

    // add an objective
    void add(ObjFun* obj_ptr) { obj_vec_.push_back(obj_ptr); }

    // Ft(u)
    double getVal(const int u) const { return 0; }

    // Ft(S)
    double getVal(const std::vector<int>& S) const { return 0; }

    // Delta(u|S)
    double getGain(const int u, const std::vector<int>& S) const { return 0; }

}; /* ObjMgr */

#endif /* __OBJ_FUN_H__ */
