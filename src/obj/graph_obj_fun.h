/**
 * Copyright (C) by J.Z. 2022-03-10 20:16
 * Distributed under terms of the MIT license.
 */

#ifndef __GRAPH_OBJ_FUN_H__
#define __GRAPH_OBJ_FUN_H__

#include "obj_fun.h"

class GraphObjFun : public ObjFun {
public:
    dir::DGraph dg_;
    int bfs_depth_;
    mutable int oracle_calls_ = 0;
    std::unordered_set<int> affected_;

public:
    GraphObjFun() {}
    GraphObjFun(const GraphObjFun& o) :affected_(o.affected_){
        oracle_calls_=0;
    }

    void add(const Action& a);

    void clear(const bool deep = false) {}
    void revBFS(const int u);
    double getVal(const std::vector<int>& S) const { return 0; }

    double getGain(const int v, const std::vector<int>& S) const { return 0; }

}; /* GraphObjFun */


//isNode function from cpplib/graph,need test
void GraphObjFun::add(const Action &a) {
    int u=a.u;
    int v=a.v;
    if(u!=v&&!dg._isEdge(u,v)){
        dg_.addEdge(u,v);
        if(!dg_.isNode(u)&&!dg_.isNode(v)){ //both u and v are new
            affected_.insert(u);
            affected_.insert(v);
        }else if(dg_.isNode(u)){ //v is new
            affected_.insert(v);
            revBFS(u);//start BFS from node u
        }else if(dg_.isNode(v)) { //u is new
            affected_.insert(u);
        }
    }
}

void GraphObjFun::revBFS(const int u) {
    affected_.insert(u);
    std::queue<int> q;
    std::unordered_set<int> visited{u};
    q.push(u);

    while(!q.empty()){
        int cv=q.front();
        q.pop();
        const auto& nd=dag_[u];
        for(auto ni=nd.beginInNbr();ni!=nd.endInNbr;ni++){
            int cu=nd.getNbrID(ni);
            if(visited.find(cu)==visited.end()){
                visited.insert(cu);
                affected_.insert(cu);
                q.push(cu);
            }
        }
    }
}

double GraphObjFun::getGain(const int v, const std::vector<int> &S) const {

}

double GraphObjFun::getVal(const std::vector<int> &S) const {
    if(s.size()==0)return 0;

    std::unordered_set<int> scope;
    for(int u:S){
        if(dg_.isNode(u)){
            std::queue<int> q;
            q.push(q);
            std::unordered_set<int> visited{u};
            while (!q.empty()){
                int cv=q.front();
                q.pop();
                const auto& nd=dag_[u];
                //bfs
            }

        }
    }
}

double GraphObjFun::clear(const bool deep) {
    oracle_calls_=0;
    affected_.clear();
    if(deep){
        dg_.clear();

    }
}
#endif /* __GRAPH_OBJ_FUN_H__ */
