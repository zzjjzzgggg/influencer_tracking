//
// Created by weiwei on 2021/7/1.
//

#ifndef SIMPLE_GREEDY_H
#define SIMPLE_GREEDY_H

#include "obj_mgr.h"

template<typename Fun>
class SimpleGreedy {
private:
    class Elem {
    public:
        int t, node;  // t: the round number that gain is updated
        double gain;

    public:
        Elem() : node(-1), t(-1), gain(-1) {}
        Elem(int v, int s = 0, double g = std::numeric_limits<double>::max())
                : node(v), t(s), gain(g) {}
    };

private:
    const ObjMgr<Fun>* obj_ptr_;
    int budget_;

    std::vector<int> chosen_;

public:
    SimpleGreedy(const ObjMgr<Fun>* input, const int budget)
            : obj_ptr_(input), budget_(budget) {
        chosen_.reserve(budget_);
    }

    /**
     * Greedy on a set of elements, return final reward.
     */
    double run(std::unordered_set<int> users) {
        chosen_.clear();
        auto cmp = [](Elem& a, Elem& b) { return a.gain < b.gain; };
        std::priority_queue<Elem, std::vector<Elem>, decltype(cmp)> pq(cmp);

        for(int u:users){
            pq.emplace(u);
        }

        double rwd = 0;
        int t = 1;
        while (!pq.empty() && t <= budget_) {
            Elem e = pq.top();
            pq.pop();
            if (e.t == t) {
                chosen_.push_back(e.node);
                t++;
                rwd += e.gain;
            } else {
                e.gain = obj_ptr_->getGain(e.node, chosen_);
                e.t = t;
                pq.push(std::move(e));
            }
        }
        return rwd;
    }

    int getOracleCalls() const { return obj_ptr_->getOracleCalls(); }

};/* SimpleGreedy */


#endif //SIMPLE_GREEDY_H
