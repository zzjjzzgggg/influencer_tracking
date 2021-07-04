//
// Created by weiwei on 2021/7/1.
//

#ifndef GREEDY_ALG_H
#define GREEDY_ALG_H

#include "obj_mgr.h"

template<typename Fun>
class GreedyAlg {
private:
    class Elem {
    public:
        int t, node;  // t: the round number that gain is updated
        double gain;

    public:
        Elem() : node(-1), t(-1), gain(-1) {}
        Elem(int v, int s = 0, double g = std::numeric_limits<double>::max())
                : node(v), t(s), gain(g) {}

        void echo() const { printf("i:%3d g:%.2e\n", t, gain); }
    };

private:
    const ObjMgr<Fun>* obj_ptr_;
    int budget_;
    std::vector<int> chosen_;

public:
    GreedyAlg(const ObjMgr<Fun>* input, const int budget)
            : obj_ptr_(input), budget_(budget) {
        chosen_.reserve(budget_);
    }

    /**
     * Run Greedy Algorithm on data provided by ObjMgr.
     * Return reward.
     */
    double run(const std::unordered_map<int, ISet>& isets,std::map<int,Action> this_action) {
        chosen_.clear();
        auto cmp = [](Elem& a, Elem& b) { return a.gain < b.gain; };
        std::priority_queue<Elem, std::vector<Elem>, decltype(cmp)> pq(cmp);

        std::unordered_set<int> users;
        for(auto item:isets){
            int u=this_action[item.first].u;
            int v=this_action[item.first].v;
            if(users.find(u)==users.end()){
                users.insert(u);
                pq.emplace(u);
            }
            if(users.find(v)==users.end()){
                users.insert(v);
                pq.emplace(v);
            }
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

}; /* GreedyAlg */

#endif //GREEDY_ALG_H
