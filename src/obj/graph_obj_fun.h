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
    static const int bfs_depth_=3;
    mutable int oracle_calls_ = 0;
    std::unordered_set<int> affected_;

public:
    GraphObjFun() {}
    GraphObjFun(const GraphObjFun& o) :affected_(o.affected_),dg_(o.dg_){
        oracle_calls_=0;
    }

    void add(const Action& a);

    void clear(const bool deep=false){
        affected_.clear();
        oracle_calls_=0;
        if(deep){
            dg_.clear();
        }
    }
    void revBFS(const int u);
    double getVal(const int u)const;
    double getVal(const std::vector<int>& S) const;
    double getGain(const int v, const std::vector<int>& S) const ;


}; /* GraphObjFun */


//isNode function from cpplib/graph,need test
void GraphObjFun::add(const Action &a) {
    int u=a.u;
    int v=a.v;

    //omit self-loop and edge already in DAG
    if(u!=v&&!dg_.isEdge(u,v)){
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

//reverse BFS
void GraphObjFun::revBFS(const int u) {
    affected_.insert(u);
    std::queue<int> q;
    std::unordered_set<int> visited{u};
    q.push(u);

    int flag=0;
    while(!q.empty()){
        flag++;
        int cv=q.front();
        q.pop();
        const auto& nd=dg_[u];
        for(auto ni=nd.beginInNbr();ni!=nd.endInNbr();ni++){
            int cu=nd.getNbrID(ni);
            if(visited.find(cu)==visited.end()){
                visited.insert(cu);
                affected_.insert(cu);
                q.push(cu);
            }
        }
        if(flag==bfs_depth_){
            break;
        }
    }
}

double GraphObjFun::getVal(const int u) const {
    std::unordered_set<int> scope;
    std::unordered_set<int> visited;
    if(dg_.isNode(u)&&(visited.find(u)!=visited.end())){
        std::queue<int> q;
        q.push(u);
        visited.insert(u);
        while (!q.empty()){
            int u=q.front();
            q.pop();
            const auto& nd=dg_[u];
            for(auto ni=nd.beginOutNbr();ni!=nd.endOutNbr();ni++){
                int cu=nd.getNbrID(ni);
                if(visited.find(cu)==visited.end()){
                    visited.insert(cu);
                    scope.insert(cu);
                    q.push(cu);
                }
            }
        }
    }
    return scope.size();
}

double GraphObjFun::getGain(const int v, const std::vector<int> &S) const {
    if(!dg_.isNode(v)){
        return 0;
    }

    ++oracle_calls_;
    if(S.size()==0){
        return getVal(v);
    }

    std::unordered_set<int> scope;
    std::unordered_set<int> visited;
    for(int u:S){
        scope.insert(u);
        if(dg_.isNode(u)&&(visited.find(u)!=visited.end())){
            std::queue<int> q;
            q.push(u);
            visited.insert(u);
            while (!q.empty()){
                int v=q.front();
                q.pop();
                const auto& nd=dg_[u];
                for(auto ni=nd.beginOutNbr();ni!=nd.endOutNbr();ni++){
                    int cu=nd.getNbrID(ni);
                    if(visited.find(cu)==visited.end()){
                        visited.insert(cu);
                        scope.insert(cu);
                        q.push(cu);
                    }
                }
            }
        }
    }
    int val=scope.size();
    std::queue<int> q;
    q.push(v);
    visited.insert(v);
    while (!q.empty()){
        int u=q.front();
        q.pop();
        const auto& nd=dg_[u];
        for(auto ni=nd.beginOutNbr();ni!=nd.endOutNbr();ni++){
            int cu=nd.getNbrID(ni);
            if(visited.find(cu)==visited.end()){
                visited.insert(cu);
                scope.insert(cu);
                q.push(cu);
            }
        }
    }
    return scope.size()-val;
}

double GraphObjFun::getVal(const std::vector<int> &S) const {
    if(S.size()==0)return 0;

    std::unordered_set<int> scope;
    std::unordered_set<int> visited;

    //use bfs
    for(int u:S){
        scope.insert(u);
        if(dg_.isNode(u)&&(visited.find(u)!=visited.end())){
            std::queue<int> q;
            q.push(u);
            visited.insert(u);

            while (!q.empty()){
                int v=q.front();
                q.pop();
                const auto& nd=dg_[u];
                for(auto ni=nd.beginOutNbr();ni!=nd.endOutNbr();ni++){
                    int cu=nd.getNbrID(ni);
                    if(visited.find(cu)==visited.end()){
                        visited.insert(cu);
                        scope.insert(cu);
                        q.push(cu);
                    }
                }
            }
        }
    }
    return scope.size();
}


#endif /* __GRAPH_OBJ_FUN_H__ */
