//
// Created by weiwei on 2021/4/20.
//

#ifndef INFLUENCERS_TRACKING_OBJ_FUN_H
#define INFLUENCERS_TRACKING_OBJ_FUN_H

#include "stdafx.h"

class ObjFun {
public:
    virtual ~ObjFun() {}

    virtual double getVal(const int u) const = 0;
    virtual double getVal(const std::vector<int>&) const = 0;
    virtual double getGain(const int, const std::vector<int>&) const = 0;

}; /* ObjFun */


#endif //INFLUENCERS_TRACKING_OBJ_FUN_H
