/**
 * Copyright (C) by J.Z. 2022-03-10 20:16
 * Distributed under terms of the MIT license.
 */

#ifndef __GRAPH_OBJ_FUN_H__
#define __GRAPH_OBJ_FUN_H__

#include "obj_fun.h"

class GraphObjFun : public ObjFun {
public:
    mutable int oracle_calls_ = 0;
    std::unordered_set<int> affected_;

public:
    GraphObjFun() {}
    GraphObjFun(const GraphObjFun& o) {}

    void add(const Action& a) {}

    void clear(const bool deep = false) {}

    double getVal(const std::vector<int>& S) const { return 0; }

    double getGain(const int v, const std::vector<int>& S) const { return 0; }

}; /* GraphObjFun */

#endif /* __GRAPH_OBJ_FUN_H__ */
