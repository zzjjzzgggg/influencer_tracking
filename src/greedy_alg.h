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
    double run(std::unordered_set<int> users) {
        auto cmp = [](Elem& a, Elem& b) { return a.gain < b.gain; };
        std::priority_queue<Elem, std::vector<Elem>, decltype(cmp)> pq(cmp);

        for(auto& u:users)pq.emplace(u);

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

    double run_alg(std::unordered_set<int> users){
        std::vector<int> S;
        int mx_gain_u;
        for(int i=0;i<budget_;i++){
            double gain_mx=-100;
            for(auto &u:users){
                double gain=obj_ptr_->getGain(u,S);
                if(gain>gain_mx){
                    gain_mx=gain;
                    mx_gain_u=u;
                }
            }
            S.push_back(mx_gain_u);
            users.erase(mx_gain_u);
            if(users.empty()){
                break;
            }
        }
        double rwd_mx=obj_ptr_->getVal(S);
        return  rwd_mx;
    }

}; /* GreedyAlg */

#endif //GREEDY_ALG_H
