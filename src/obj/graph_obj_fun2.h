//
// Created by zhangwei on 2022/3/22.
//

#ifndef __GRAPH_OBJ_FUN2_H__
#define __GRAPH_OBJ_FUN2_H__

#include "obj_fun.h"

class GraphObjFun:public ObjFun,public HyperANF{
public:
    mutable int oracle_calls_=0;
    dir::DGraph dag_;
    std::unordered_set<int> affected_;
    mutable lru::Cache<std::string, double> cache_{5, 0};
private:
    // Get the CC node u belongs to; otherwise return u itself as the CC.
    int getCC(const int u);

    // Assign cc a position in bits_ vector and return the pos.
    int newPos(const int cc);

    inline bool exists(const int cc) const {
        return cc_bitpos_.find(cc) != cc_bitpos_.end();
    }

    bool existsNode(const int u) const {
        if (nd_cc_.find(u) == nd_cc_.end()) return false;
        return cc_bitpos_.find(nd_cc_.at(u)) != cc_bitpos_.end();
    }

    // return true if c1 >= c2 in decreading topological order
    inline bool isGEq(const int c1, const int c2) const {
        return isGreaterEqual(cc_bitpos_.at(c1), cc_bitpos_.at(c2));
    }

    // merge counter of c2 to counter c1
    inline void mergeCC(const int c1, const int c2) {
        mergeCounter(cc_bitpos_.at(c1), cc_bitpos_.at(c2));
    }

    inline void genCounter(const int cc) { genHLLCounter(newPos(cc)); }

    void revBFS(const int cu);

    template <class InputIter>
    double getGain(const int u, const InputIter first,
                   const InputIter last) const;

public:
    GraphObjFun(const int p = 10) : HyperANF(p), ObjFun() {}

    // copy constructor
    GraphObjFun(const GraphObjFun& o)
        : ObjFun(o), HyperANF(o), dag_(o.dag_) {}

    // copy assignment
    GraphObjFun& operator=(const GraphObjFun& o) {
        ObjFun::operator=(o);
        HyperANF::operator=(o);
        dag_ = o.dag_;
        return *this;
    }
    void add(const Action &a){
        addEdge(a.u,a.v);
    }
    // addEdge will record new CC connections to new_cc_edges_.
    void addEdge(const int u, const int v);

    void clear(const bool deep = false);
    std::vector<int> getNodes() const {
        std::vector<int> nodes;
        for (auto& pr : nd_cc_) nodes.push_back(pr.first);
        return nodes;
    }

    double getVal(const int node) const  {
        if(!existsNode(node))return 0;
        return estimate(node);
    }
    double getVal(const std::vector<int>& S) const  {
        std::vector<int> newS;
        for(int u:S){
            if(existsNode(u)){
                newS.push_back(u);
            }
        }
        return estimate(newS.begin(), newS.end());
    }

    double getGain(const int u, const std::vector<int>& S) const ;


    void debug() const  {
        printf("graph: (%d, %d)\n", dag_.getNodes(), dag_.getEdges());
        graph::saveEdgelist(dag_, "edges_v2.dat");
    }

}; 

// implementations
double GraphObjFun::getGain(const int u, const std::vector<int> &S)const {
    std::string key = "{}"_format(u);
    for (auto it = S.begin(); it != S.end(); ++it) key.append("|{}"_format(*it));
    if (cache_.contains(key)) return cache_.get(key);

    if(!existsNode(u)) return 0;


    ++oracle_calls_;
    double gain = 0;
    if (S.size()==0)
        gain = getVal(u);
    else {
        double rwd_S = 0;
        std::vector<uint64_t> tmp_bits(units_per_counter_, 0);
        for (auto it = S.begin(); it != S.end(); ++it) {
            if (existsNode(*it))
                mergeCounter(tmp_bits.data(), cc_bitpos_.at(nd_cc_.at(*it)));
        }
        rwd_S = hll::count((uint8_t*)tmp_bits.data(), m_);
        mergeCounter(tmp_bits.data(), getCounterPos(u));

        gain = hll::count((uint8_t*)tmp_bits.data(), m_) - rwd_S;
    }

    cache_.insert(key, gain);
    return gain;
}

int GraphObjFun::getCC(const int u) {
    if (nd_cc_.find(u) != nd_cc_.end())
        return nd_cc_.at(u);
    else
        nd_cc_[u] = u;
    return u;
}

int GraphObjFun::newPos(const int cc) {
    int pos = bits_.size();
    bits_.resize(pos + units_per_counter_, 0);
    cc_bitpos_[cc] = pos;
    return pos;
}

void GraphObjFun::revBFS(const int cv) {
    affected_.insert(cv);
    std::queue<int> queue;
    std::unordered_set<int> visited{cv};
    queue.push(cv);
    while (!queue.empty()) {
        int cv = queue.front();
        queue.pop();
        const auto& nd = dag_[cv];
        for (auto ni = nd.beginInNbr(); ni != nd.endInNbr(); ++ni) {
            int cu = nd.getNbrID(ni);
            if (visited.find(cu) == visited.end()) {
                visited.insert(cu);
                if (!isGEq(cu, cv)) {
                    mergeCC(cu, cv);
                    affected_.insert(cu);
                    queue.push(cu);
                }
            }
        }
    }
}

void GraphObjFun::addEdge(const int u, const int v) {
    int cu = getCC(u), cv = getCC(v);
    // omit self-loop edges and edges already in DAG
    if (cu != cv && !dag_.isEdge(cu, cv)) {
        dag_.addEdge(cu, cv);
        if (!exists(cu) && !exists(cv)) {  // both u and v are new
            genCounter(cu);
            genCounter(cv);
            mergeCC(cu, cv);
            affected_.insert(cu);
            affected_.insert(cv);
        } else if (exists(cu)&&!exists(cv)) {  // v is new
            genCounter(cv);
            affected_.insert(cv);
            mergeCC(cu, cv);
            revBFS(cu);
        } else if (exists(cv)&& !exists(cu)) {  // u is new
            genCounter(cu);
            mergeCC(cu, cv);
            affected_.insert(cu);
        }
//        } else if (!isGEq(cu, cv)) {  // both exist and cu is not higher than cv
//            mergeCC(cu, cv);
//            revBFS(cu);
//        }
            else if(exists(cv)&& exists(cu)){
                mergeCC(cu, cv);
                revBFS(cu);
            }
        }

}

void GraphObjFun::clear(const bool deep) {
    oracle_calls_=0;
    affected_.clear();
    if (deep) {
        HyperANF::clear();
        dag_.clear();
        cache_.clear();
    }
}


#endif  // __GRAPH_OBJ_FUN2_H__
