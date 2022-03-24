//
// Created by zhangwei on 2021/7/1.
//

#ifndef EVAL_STREAM_H
#define EVAL_STREAM_H

#include "stdafx.h"
#include "iset_segment.h"
#include "obj/obj_mgr.h"

template <typename Fun,typename InputData>
class EvalStream {
private:
    int L_, cur_ = 0;
    std::vector<std::vector<std::pair<int, ISet>>> buf_;  // action_index->iset
    std::map<int, InputData> this_action_;                   // index->action

public:
    EvalStream(const int L) : L_(L) { buf_.resize(L); }

    void add(const InputData& a, const ISetSegments& segs) {
        for (auto& seg : segs.segments_)
            for (int i = seg.start_; i < seg.end_; ++i) {
                buf_[(cur_ + i) % L_].emplace_back(a.t, seg.is_);
                this_action_[a.t] = a;  // identify action a with index
            }
    }

    void next() {
        buf_[cur_].clear();
        cur_ = (cur_ + 1) % L_;
    }

    /**
     * Use current actions update ObjMgr
     */
    const ObjMgr<Fun,InputData> getObjMgr(int n) {
        ObjMgr<Fun,InputData> obj_mgr_(n);
        for (auto& pr : buf_[cur_])
            obj_mgr_.update(this_action_[pr.first], pr.second);
        return obj_mgr_;
    }

    /**
     * Get current users
     */
    std::unordered_set<int> get_users() {
        std::unordered_set<int> users;
        for (auto& pr : buf_[cur_]) {
            auto a = this_action_[pr.first];
            users.insert(a.u);
            users.insert(a.v1);
            users.insert(a.v2);
        }
        return users;
    }

    /**
     * Get current locations
     */
    std::unordered_set<int> get_locs() {
        std::unordered_set<int> locations;
        for (auto& pr : buf_[cur_]) {
            auto a = this_action_[pr.first];
            locations.insert(a.u);
        }
        return locations;
    }

}; /* EvalStream */

#endif  // EVAL_STREAM_H
