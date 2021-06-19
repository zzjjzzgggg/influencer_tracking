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
    CheckinObjFun();

    void add(const Action& a);
    void clear();

    double getVal(const int u) const;
    double getVal(const std::vector<int>& S) const;
    double getGain(const int v, const std::vector<int>& S) const;

}; /* CheckinObjFun */

#endif /* __CHECKIN_OBJ_FUN_H__ */
