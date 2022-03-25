/**
 * Copyright (C) by J.Z. 2022-03-10 20:16
 * Distributed under terms of the MIT license.
 */

#ifndef __GRAPH_OBJ_FUN_H__
#define __GRAPH_OBJ_FUN_H__

#include "obj_fun.h"

class GraphObjFun : public ObjFun {
public:
    dir::DGraph graph_;
    static const int bfs_depth_ = 3;
    mutable int oracle_calls_ = 0;
    std::unordered_set<int> affected_;

public:
    GraphObjFun() {}
    GraphObjFun(const GraphObjFun& o) : affected_(o.affected_), graph_(o.graph_) {
        oracle_calls_ = 0;
    }

    void add(const TAction& a);
    void addEdge(const int u, const int v);
    void clear(const bool deep = false) {
        affected_.clear();
        oracle_calls_ = 0;
        if (deep) graph_.clear();
    }

    double getVal(const int u) const override;
    double getVal(const std::vector<int>& S) const override;
    double getGain(const int v, const std::vector<int>& S) const override;

}; /* GraphObjFun */

void GraphObjFun::add(const TAction& a) {
    int u = a.u;
    int v1 = a.v1;
    int v2 = a.v2;
    addEdge(u, v1);
    addEdge(v1, v2);
}

void GraphObjFun::addEdge(const int u, const int v) {
    // omit self-loop and edge already in the graph
    if (u == v || graph_.isEdge(u, v)) return;

    bool u_exist = graph_.isNode(u), v_exist = graph_.isNode(v);
    graph_.addEdge(u, v);

    // both nodes u and v are new
    if (!u_exist && !v_exist) {
        affected_.insert(u);
        affected_.insert(v);
        return;
    }

    // only node u is new
    if (!u_exist && v_exist) {
        affected_.insert(u);
        return;
    }

    // only node v is new (need a reverse BFS from node u)
    if (u_exist && !v_exist) affected_.insert(v);

    // also if both nodes already exist, we need a reverse BFS from node u

    // perform a reverse BFS from node u for (bfs_depth_ - 1) hops.
    graph::DirBFS<graph::dir::DGraph> bfs(graph_);
    bfs.doRevBFS(u, bfs_depth_ - 1);
    for (auto it : bfs.nd_to_hop_) affected_.insert(it.first);
}

double GraphObjFun::getVal(const int u) const {
    if (!graph_.isNode(u)) return 0;
    graph::DirBFS<graph::dir::DGraph> bfs(graph_);
    bfs.doBFS(u, bfs_depth_);
    return bfs.getBFSTreeSize();
}

double GraphObjFun::getVal(const std::vector<int>& S) const {
    if (S.size() == 0) return 0;
    graph::DirBFS<graph::dir::DGraph> bfs(graph_);
    bfs.doBFS(S.begin(), S.end(), bfs_depth_);
    return bfs.getBFSTreeSize();
}

double GraphObjFun::getGain(const int v, const std::vector<int>& S) const {
    if (!graph_.isNode(v)) return 0;
    ++oracle_calls_;
    if (S.size() == 0) return getVal(v);

    graph::DirBFS<graph::dir::DGraph> bfs(graph_);
    return bfs.doIncBFS(S.begin(), S.end(), v, bfs_depth_).size();
}

#endif /* __GRAPH_OBJ_FUN_H__ */
