//
// Created by weiwei on 2021/4/18.
//

#ifndef SIEVEPAG_SIEVE_PAG_H
#define SIEVEPAG_SIEVE_PAG_H

#endif //SIEVEPAG_SIEVE_PAG_H

#include<candidate.h>
#include "social_action.h"

class SievePAG{
private:
    std::vector<SocialAction> social_actions_;
    int budget_;
    double eps_;
    double delta;
    std::vector<double> thresholds_;
    std::vector<Candidate> candidate_buf_;
    std::map<int,int> this_pos_;
//    std::vector<int> recycle_;
private:
    //theta
    inline double getThreshold(const int i)const{
        return std::pow(1+eps_,i)/(2*budget_);
    }

    //for a threshold index i,return candidate c
    inline const Candidate& getCandidate(const int i)const {
        return candidate_buf_[this_pos_.at(i)]
    }

    // Add a new threshold indexede by i
    void addTheta(const int i);
    void delTheta(const int i);
    void updateThresholds(std::vector<int> new_thresholds);

public:
    SievePAG(const int budget,const double eps):budget_(budget),eps_(eps){}

    SievePAG& operator=(const SievePAG& s){
        budget_=s.budget_;
        eps_=s.eps_;
        candidate_buf_=s.candidate_buf_;
        this_pos_=s.this_pos_;
    }
    void clear(){
        thresholds_.clear();
        this_pos_.clear();
        for(auto& cad:candidate_buf_){
            cad.clear();
        }
    }

    //Process social action e and its Bernoulli set
    void feed(const SocialAction socialAction,const BernoulliSet& bs);

    //Get the number of thresholds maintained in SievePag
    int size() const {return this_pos_.size()}

    void buildGraph();
    double getMaxDelta();
    std::vector<int>  getInfluencers(SocialAction e);
    void filterNodes();
    double getGain(std::vector<int> affect_nodes);
};
