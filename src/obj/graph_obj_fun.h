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
    void BFS(const int u,std::unordered_set<int>& nodes) const;
    void BFS(const std::vector<int>& S,std::unordered_set<int>& nodes) const;
    void revBFS(const int u);
    double getVal(const int u)const;
    double getVal(const std::vector<int>& S) const;
    double getGain(const int v, const std::vector<int>& S) const ;

}; /* GraphObjFun */

//find the nodes are reachable from node u
void GraphObjFun::BFS(const int u, std::unordered_set<int>& nodes) const {
    if(dg_.isNode(u)){
        std::queue<int> q;
        q.push(u);
        nodes.insert(u);
        while (!q.empty()){
            int u=q.front();
            q.pop();
            const auto& nd=dg_[u];
            for(auto ni=nd.beginOutNbr();ni!=nd.endOutNbr();ni++){
                int cu=nd.getNbrID(ni);
                if(nodes.find(cu)==nodes.end()){
                    nodes.insert(cu);
                    q.push(cu);
                }
            }
        }
    }
}

//find the nodes are reachable from node set S
void GraphObjFun::BFS(const std::vector<int>& S, std::unordered_set<int>& nodes) const {
    for(int u:S){
        if(dg_.isNode(u)&&(nodes.find(u)==nodes.end())){
            std::queue<int> q;
            q.push(u);
            nodes.insert(u);
            while (!q.empty()){
                int u=q.front();
                q.pop();
                const auto& nd=dg_[u];
                for(auto ni=nd.beginOutNbr();ni!=nd.endOutNbr();ni++){
                    int cu=nd.getNbrID(ni);
                    if(nodes.find(cu)==nodes.end()){
                        nodes.insert(cu);
                        q.push(cu);
                    }
                }
            }
        }
    }
}

void GraphObjFun::add(const Action &a) {
    int u=a.u;
    int v=a.v;

    //omit self-loop and edge already in DAG
    if(u!=v&&!dg_.isEdge(u,v)){
        bool u_exist=dg_.isNode(u);
        bool v_exist=dg_.isNode(v);
        dg_.addEdge(u,v);
        if(!u_exist&&!v_exist){ //both u and v are new
            affected_.insert(u);
            affected_.insert(v);
        }else if(u_exist&&!v_exist){ //v is new
            affected_.insert(v);
            revBFS(u);//start reverse BFS from node u
        }else if(!u_exist&&v_exist) { //u is new
            affected_.insert(u);
        }else{//both u and v exist
            revBFS(u);
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
        int n=q.size();
        flag++;//flag for exit BFS
        for(int i=0;i<n;i++){
            int u=q.front();
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
        }
        if(flag==bfs_depth_){
            break;
        }
    }
}

double GraphObjFun::getVal(const int u) const {
    std::unordered_set<int> scope;
    BFS(u,scope);
    return scope.size();
}

double GraphObjFun::getVal(const std::vector<int> &S) const {
    if(S.size()==0)return 0;
    std::unordered_set<int> scope;
    BFS(S,scope);
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
    BFS(S,scope);
    int val=scope.size();
    BFS(v,scope);
    return scope.size()-val;
}

#endif /* __GRAPH_OBJ_FUN_H__ */
