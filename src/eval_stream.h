//
// Created by zhangwei on 2021/7/1.
//

#ifndef EVAL_STREAM_H
#define EVAL_STREAM_H

#include "stdafx.h"
#include "iset_segment.h"
#include "obj/obj_mgr.h"

template <typename Fun>
class EvalStream {
private:
    int L_, cur_ = 0;
    // buf_ stores the I-sets of active social actions at each time step
    std::vector<std::vector<std::pair<Action, ISet>>> buf_;

public:
    EvalStream(const int L) : L_(L) { buf_.resize(L); }

    void add(const Action& a, const ISetSegments& segs) {
        for (auto& seg : segs.segments_)
            for (int i = seg.start_; i < seg.end_; ++i)
                buf_[(cur_ + i) % L_].emplace_back(a, seg.is_);
    }

    void next() {
        buf_[cur_].clear();
        cur_ = (cur_ + 1) % L_;
    }

    /**
     * Use current actions update ObjMgr
     */
    const ObjMgr<Fun> getObjMgr(int n) {
        ObjMgr<Fun> obj_mgr(n);
        for (auto& pr : buf_[cur_]) obj_mgr.update(pr.first, pr.second);
        return obj_mgr;
    }

    /**
     * Get current active nodes. If both=false, only return the nodes from this
     * first field of Action, which is usefull to hanle check-in data.
     */
    std::unordered_set<int> getNodes(const bool both = true) {
        std::unordered_set<int> users;
        for (auto& pr : buf_[cur_]) {
            auto a = pr.first;
            users.insert(a.u);
            if (both) users.insert(a.v);
        }
        return users;
    }

}; /* EvalStream */

#endif  // EVAL_STREAM_H
