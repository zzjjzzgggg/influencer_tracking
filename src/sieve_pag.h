//
// Created by weiwei on 2021/4/18.
//

#ifndef SIEVE_PAG_H
#define SIEVE_PAG_H


#include "dyn_dgraph_mgr_v2.h"
#include "candidate.h"

class SievePAG{
public:
    int num_samples_;   //n
    int budget_;     //k
    double eps_;  //epsilon
    double mx_gain_=0; //max_gain

    DynDGraphMgr input_mgr_;

    std::vector<DynDGraphMgr*> sam_graphs_;  //store the n sample graphs

    std::vector<Candidate> candidate_buf_; //store the candidate

    std::map<int,int> this_pos_;//theta_index->buf_position
    std::stack<int> recycle_bin_;  //a vector of positions for recycling

private:
    //theta = (1+\epsilon)^i / (2k)
    inline double getThreshold(const int i)const{
        return std::pow(1+eps_,i)/(2*budget_);
    }

    // Add a new threshold indexed by i
    void addTheta(const int i);

    //delete a new threshold indexed by i
    void delTheta(const int i);

    // Given a threshold index i, return candidate c.
    inline const Candidate& getCandidate(const int i) const {
        return candidate_buf_[this_pos_.at(i)];
    }

    //for a threshold index i,return candidate c
    inline Candidate & getCandidate(const int i) {
        return candidate_buf_[this_pos_.at(i)];
    }

public:
    SievePAG(const int num_samples,const int budget,const double eps):num_samples_(num_samples),budget_(budget),eps_(eps)
            {
        for(int i=0;i<num_samples;i++){
            DynDGraphMgr *sg=new DynDGraphMgr();
            sam_graphs_.push_back(sg);
        }
    }

    SievePAG& operator=(const SievePAG& s){
        num_samples_=s.num_samples_;
        budget_=s.budget_;
        eps_=s.eps_;
        mx_gain_ = s.mx_gain_;
        candidate_buf_=s.candidate_buf_;
        this_pos_=s.this_pos_;
        recycle_bin_ = s.recycle_bin_;
        return *this;
    }

    void clear(const bool deep=false){
        if(deep){
            mx_gain_=0;
            this_pos_.clear();
            while (!recycle_bin_.empty()) recycle_bin_.pop();
            for(auto& cad:candidate_buf_){
                cad.clear();
            }
        }
        input_mgr_.clear(deep);
        for(int i=0;i<num_samples_;i++){
            sam_graphs_[i]->clear(deep);
        }
    }

    //Process social action e and its Bernoulli set
    void update(const SocialAc &s,const BernoulliSet& bs);

    // Get current maximum reward.
//    std::vector<int> getResult() ;
    double getResult();
    void updateThresholds();

    bool updateMaxGain(const std::vector<int> &nodes);

};


#endif //SIEVE_PAG_H
