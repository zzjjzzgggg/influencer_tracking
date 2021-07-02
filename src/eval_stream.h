//
// Created by weiwei on 2021/7/1.
//

#ifndef EVAL_STREAM_H
#define EVAL_STREAM_H

#include "stdafx.h"
#include "iset_segment.h"
#include "obj_mgr.h"

template<typename Fun>
class EvalStream {
private:
    int L_, cur_ = 0;
    std::vector<std::vector<std::pair<int, ISet>>> buf_; //action_index->iset
    std::map<int,Action> this_action_;//index->action
public:
    EvalStream(const int L) : L_(L) { buf_.resize(L); }

    void add(const Action &a, const ISetSegments& segs) {
        for (auto& seg : segs.segments_)
            for (int i = seg.start_; i < seg.end_; ++i){
                buf_[(cur_ + i) % L_].emplace_back(a.t, seg.is_);
                this_action_[a.t]=a;//identify action a with index
            }
    }

    void next() {
        buf_[cur_].clear();
        cur_ = (cur_ + 1) % L_;
    }

    /**
     * Use current actions update ObjMgr
     */
    const  ObjMgr<Fun> getObjMgr(int n) {
        ObjMgr<Fun> obj_mgr_(n);
        for (auto& pr:buf_[cur_])
            obj_mgr_.update(this_action_[pr.first],pr.second);
        return obj_mgr_;
    }

    /**
     * Get Actions
     */
    std::map<int,Action> get_Actions(){
        return std::map(this_action_.begin(),this_action_.end());
    }

    /**
     * Get current ISets
     */
    std::unordered_map<int, ISet> getPop() const {
        std::unordered_map<int, ISet> pop;
        for (auto& pr : buf_[cur_]) pop[pr.first] = std::move(pr.second);
        return pop;
    }

}; /* EvalStream */


#endif //EVAL_STREAM_H
