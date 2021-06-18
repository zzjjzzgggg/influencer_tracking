//
// Created by weiwei on 2021/5/27.
//

#ifndef INFLUENCERS_TRACKING_SIEVE_PAIT_H
#define INFLUENCERS_TRACKING_SIEVE_PAIT_H

#include "candidate.h"
#include "obj_fun.h"

class SievePAIT {
public:
    int num_samples_, budget_;  // n and k
    double eps_, mx_gain_ = 0;  // epsilon and max gain

    const ObjMgr *obj_mgr_ptr_;

    std::vector<Candidate> candidate_buf_;  // store the candidate

    std::map<int, int> thi_pos_;   // theta_index->buf_position
    std::stack<int> recycle_bin_;  // a vector of positions for recycling

private:
    // theta = (1+\epsilon)^i / (2k)
    inline double getThreshold(const int i) const {
        return std::pow(1 + eps_, i) / (2 * budget_);
    }

    // Add a new threshold indexed by i
    void addTheta(const int i);

    // delete a new threshold indexed by i
    void delTheta(const int i);

    // Given a threshold index i, return candidate c.
    inline const Candidate &getCandidate(const int i) const {
        return candidate_buf_[thi_pos_.at(i)];
    }

    // for a threshold index i, return candidate c
    inline Candidate &getCandidate(const int i) {
        return candidate_buf_[thi_pos_.at(i)];
    }

public:
    SievePAIT(const int num_samples, const int budget, const double eps,
              const ObjMgr *obj_mgr_ptr)
        : num_samples_(num_samples), budget_(budget), eps_(eps),
          obj_mgr_ptr_(obj_mgr_ptr) {
        // |\Theta| = O(\epsilon^{-1}\log 2k)
        candidate_buf_.reserve((int)(std::log2(2 * budget_) / eps_));
    }

    SievePAIT(const SievePAIT &o)
        : num_samples_(o.num_samples_), budget_(o.budget_), eps_(o.eps_),
          mx_gain_(o.mx_gain_), obj_mgr_ptr_(o.obj_mgr_ptr_),
          candidate_buf_(o.candidate_buf_), thi_pos_(o.thi_pos_),
          recycle_bin_(o.recycle_bin_) {}

    SievePAIT &operator=(const SievePAIT &o) {
        num_samples_ = o.num_samples_;
        budget_ = o.budget_;
        eps_ = o.eps_;
        mx_gain_ = o.mx_gain_;
        obj_mgr_ptr_ = o.obj_mgr_ptr_;
        candidate_buf_ = o.candidate_buf_;
        thi_pos_ = o.thi_pos_;
        recycle_bin_ = o.recycle_bin_;
        return *this;
    }
    // Process social action e and its I set
    void update(const SocialAc &a, const ISet &iset);

    double getResult();
    void updateThresholds();

    void clear() {
        mx_gain_ = 0;
        thi_pos_.clear();
        while (!recycle_bin_.empty()) recycle_bin_.pop();
        for (auto &ca : candidate_buf_) ca.clear();
    }

    int getOracleCalls() {
        int oracle_calls = 0;
        // for (int i = 0; i < num_samples_; i++) {
        //     oracle_calls += sam_graphs_[i]->getOracleCalls();
        // }
        return oracle_calls;
    }
};

void SievePAIT::addTheta(const int i) {
    int pos;
    if (!recycle_bin_.empty()) {   // if recycle_bin has an unoccupied room
        pos = recycle_bin_.top();  // the last
        recycle_bin_.pop();        // delete the last one in recycle bin
        candidate_buf_[pos].clear();
    } else {                          // otherwise realloc room
        pos = candidate_buf_.size();  // add a new one in candidate_buf
        candidate_buf_.emplace_back();
    }
    thi_pos_[i] = pos;
}

void SievePAIT::delTheta(const int i) {
    // get pos
    int pos = thi_pos_[i];
    candidate_buf_[pos].clear();
    recycle_bin_.push(pos);
    thi_pos_.erase(i);
}

void SievePAIT::updateThresholds() {
    // the new_li may be the log( (1-eps)*mx_gain_) /log(1 + eps_)
    int new_li = (int)std::floor(std::log(mx_gain_) / std::log(1 + eps_)),
        new_ui =
            (int)std::ceil(std::log(2 * budget_ * mx_gain_) / std::log(1 + eps_));
    int li, ui;  // lower bound and upper bound of theata index
    if (!thi_pos_.empty()) {
        li = thi_pos_.begin()->first;   // get the theta index
        ui = thi_pos_.rbegin()->first;  // get the theta index reverse
        while (li <= ui && li < new_li) {
            delTheta(li);  // delete outdated thresholds
            ++li;
        }
    }
    li = thi_pos_.empty() ? new_li : ui + 1;
    for (int i = li; i <= new_ui; i++) addTheta(i);
}

void SievePAIT::update(const SocialAc &a, const ISet &is) {
    // get affected nodes
    std::vector<int> nodes;  // = sg_.getAffectedNodes();

    // filter nodes by thresholds
    for (auto u : nodes) {
        double val = obj_mgr_ptr_->getVal(u);
        if (val > mx_gain_) {
            mx_gain_ = val;
            updateThresholds();
        }

        for (auto &pr : thi_pos_) {
            int i = pr.first;            // theta-index
            auto &ca = getCandidate(i);  // definite cite may need to insert item
            if (!ca.isMember(u) && ca.size() < budget_) {
                double threshold = getThreshold(i);
                double gain = obj_mgr_ptr_->getGain(u, ca.getMembers());
                if (gain >= threshold) ca.insert(u);
            }
        }
    }
};

double SievePAIT::getResult() {
    int i_mx = -100;  // theta-index
    double rwd_mx = 0;
    for (auto &pr : thi_pos_) {
        double rwd_sum = 0;
        int i = pr.first;           // theta index
        auto ca = getCandidate(i);  // get the theta to index
        double rwd = obj_mgr_ptr_->getVal(ca.getMembers());
        if (rwd > rwd_mx) {
            rwd_mx = rwd;
            i_mx = i;
        }
    }
    return rwd_mx;
}

#endif  // INFLUENCERS_TRACKING_SIEVE_PAIT_H
