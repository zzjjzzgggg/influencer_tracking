//
// Created by weiwei on 2021/4/20.
//

#ifndef INPUT_MGR_H
#define INPUT_MGR_H

#include "stdafx.h"

/**
 * Abstract class InputMgr:
 *
 * Feed a batch of edges, outputs a set of nodes whose margianl gain changes.
 */
class InputMgr {
public:
    int serial;
    mutable int oracle_calls_ = 0;
    std::unordered_set<int> affected_nodes_;

public:
    InputMgr() : oracle_calls_(0) {}
    // copy constructor
    InputMgr(const InputMgr& o)
            : oracle_calls_(o.oracle_calls_), affected_nodes_(o.affected_nodes_) {}
    // copy assignment
    InputMgr& operator=(const InputMgr& o) {
        oracle_calls_ = o.oracle_calls_;
        affected_nodes_ = o.affected_nodes_;
        return *this;
    }
    virtual ~InputMgr() {}

    virtual void addEdge(const int u, const int v) = 0;
    virtual void addEdges(const IntPrV& edges) {
        for (auto& edge : edges) addEdge(edge.first, edge.second);
    }

    virtual int getOracleCalls() const { return oracle_calls_; }
    virtual std::vector<int> getAffectedNodes() {
        return std::vector<int>(affected_nodes_.begin(), affected_nodes_.end());
    }

    /**
     * If deep = true, then clear everything; otherwise only clear temporal
     * results.
     */
    virtual void clear(const bool deep = false) {
        oracle_calls_ = 0;
        affected_nodes_.clear();
    }

    /**
     * Return all nodes maintained in current input mgr
     */
    virtual std::vector<int> getNodes() const = 0;
    virtual int getNumNodes() const = 0;

    virtual double getReward(const int) const = 0;
    virtual double getReward(const std::vector<int>&) const = 0;
    virtual double getReward(const std::unordered_set<int>&) const = 0;
    virtual double getGain(const int, const std::vector<int>&) const = 0;
    virtual double getGain(const int, const std::unordered_set<int>&) const = 0;

    virtual void debug() const {}

}; /* InputMgr */

#endif //INPUT_MGR_H
