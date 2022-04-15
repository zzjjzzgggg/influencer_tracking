/**
 * Copyright (C) by J.Z. (2018-07-17 14:41)
 * Distributed under terms of the MIT license.
 */

#ifndef __DIM_STREAM_H__
#define __DIM_STREAM_H__

#include "../eval_stream.h"
#include "dim.hpp"

class DIMStream : public EvalStream {
private:
    DIM dim_;

private:
    double getProb(const ISet& iset) const { return (double)iset.size() / n_; }

public:
    DIMStream(const int L, const int n, const int beta) : EvalStream(L, n) {
        dim_.init();
        dim_.set_beta(beta);
    }

    void update() {
        std::map<Edge, double> edge_q_map;  // edge -> 1-p
        for (auto& pr : buf_[cur_]) {
            Edge edge = std::make_pair(pr.first.u, pr.first.v);
            double p = getProb(pr.second);
            if (!edge_q_map.count(edge))
                edge_q_map[edge] = 1 - p;
            else
                edge_q_map[edge] *= 1 - p;
        }

        auto edges_tuple = dim_.cmp_edges(edge_q_map);
        for (auto edge : std::get<0>(edges_tuple)) {  // delete edges
            dim_.erase(edge.first, edge.second);
        }
        for (auto edge : std::get<1>(edges_tuple)) {  // add new edges
            dim_.insert(edge.first);
            dim_.insert(edge.second);
            dim_.insert(edge.first, edge.second, 1 - edge_q_map.at(edge));
        }
        for (auto edge : std::get<2>(edges_tuple)) {  // update edges
            dim_.change(edge.first, edge.second, 1 - edge_q_map.at(edge));
        }
    }

    std::vector<int> solveIM(const int budget) { return dim_.infmax(budget); }

}; /* DIMStream */

#endif /* __DIM_STREAM_H__ */
