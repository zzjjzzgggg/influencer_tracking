//
// Created by weiwei on 2021/4/18.
//

#ifndef SIEVE_PAG_H
#define SIEVE_PAG_H


#include "candidate.h"

template<class InputMgr>
class SievePAG{
public:
    int num_samples_;   //n
    int budget_;     //k
    double eps_;  //epsilon
    double mx_gain_=0; //max_gain

    InputMgr input_mgr_;

    std::vector<InputMgr*> sam_graphs_;  //store the n sample graphs

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
    SievePAG(const int num_samples,const int budget,const double eps);

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

    //Process social action e and its I set
    void update(const SocialAc &s,const ISet & bs);

    double getResult();
    void updateThresholds();

    bool updateMaxGain(const std::vector<int> &nodes);

};


template<class InputMgr>
SievePAG<InputMgr>::SievePAG(const int num_samples,const int budget,const double eps)
{
    num_samples_=num_samples;
    budget_=budget;
    eps_=eps;
    for(int i=0;i<num_samples;i++){
        InputMgr *sg=new InputMgr();
        sam_graphs_.push_back(sg);
    }
}

/**
 *add threshold theta
 */
template<class InputMgr>
void SievePAG<InputMgr>::addTheta(const int i) {
    int pos;
    if(!recycle_bin_.empty()){//if recycle_bin has an unoccupied room
        pos=recycle_bin_.top();//the last
        recycle_bin_.pop();//delete the last one in recycle bin
        candidate_buf_[pos].clear();
    }
    else{ //otherwise realloc room
        pos=candidate_buf_.size();//add a new one in candidate_buf
        candidate_buf_.emplace_back();
    }
    this_pos_[i]=pos;
}

/**
 *delete threshold theta
 */
template<class InputMgr>
void SievePAG<InputMgr>::delTheta(const int i) {
    //get pos
    int pos=this_pos_[i];
    candidate_buf_[pos].clear();
    recycle_bin_.push(pos);
    this_pos_.erase(i);
}

/**
 * update thresholds
 */
template<class InputMgr>
void SievePAG<InputMgr>::updateThresholds() {
    //the new_li may be the log( (1-eps)*mx_gain_) /log(1 + eps_)
    int new_li =(int)std::floor(std::log( mx_gain_) / std::log(1 + eps_)),
            new_ui =(int)std::ceil(std::log(2 * budget_ * mx_gain_) / std::log(1 + eps_));
    int li, ui;               // lower bound and upper bound of theata index
    if (!this_pos_.empty()) {
        li = this_pos_.begin()->first;//get the theta index
        ui = this_pos_.rbegin()->first;//get the theta index reverse
        while (li <= ui && li < new_li) {
            delTheta(li);// delete outdated thresholds
            li++;
        }
    }
    li = this_pos_.empty() ? new_li : ui + 1;
    for (int i = li; i <= new_ui; i++) addTheta(i);
}

/**
 * check whether the delta change
 */
template<class InputMgr>
bool SievePAG<InputMgr>::updateMaxGain(const std::vector<int> &nodes) {

    bool is_changed=false;
    for(auto u:nodes){ //for all node,calculate the Ft({v})
        double reward_sums=0;
        for(int i=0;i<num_samples_;i++){
                reward_sums+=sam_graphs_[i]->getReward(u);
        }
        double reward=reward_sums/num_samples_;
        if(reward>mx_gain_){
            mx_gain_=reward;
            is_changed= true;
        }
    }
    return is_changed;
}

/**
*Process social action s and its Bernoulli set.
*/
template<class InputMgr>
void SievePAG<InputMgr>::update(const SocialAc &s,const ISet& is){
    //add edge
    input_mgr_.addEdge(s.first.first,s.first.second);
    //get affected nodes
    std::vector<int> nodes=input_mgr_.getAffectedNodes();

    for(auto i:is){
        sam_graphs_[i]->addEdge(s.first.first,s.first.second);
    }

    //if max delta change ,delta change,need to update thresholds
    if(updateMaxGain(nodes)) {
        updateThresholds();
    }

    // filter nodes by thresholds
    for(auto u:nodes){
        for(auto &pr:this_pos_){
            int i=pr.first;//theta-index
            auto& ca=getCandidate(i);//definite cite may need to insert item
            if(!ca.isMember(u)&&ca.size()<budget_){
                double gain_sums=0;//sum of gain
                double threshold=getThreshold(i);
                std::vector<int> SS=ca.getMembers();
                for(int k=0;k<num_samples_;k++){
                    gain_sums+=sam_graphs_[k]->getGain(u,SS);
                }
                double gain=gain_sums/num_samples_;
                if(gain>=threshold){
                    ca.insert(u);
                }
            }
        }
    }
};

template<class InputMgr>
double SievePAG<InputMgr>::getResult() {
    int i_mx=-100;//theta-index
    double rwd_mx=0;
    for(auto &pr:this_pos_){
        double rwd_sum=0;
        int i=pr.first;//theta index
        auto ca=getCandidate(i);//get the theta to index
        std::vector<int> SS=ca.getMembers();
        for(int k=0;k<num_samples_;k++) {
            rwd_sum += sam_graphs_[k]->getReward(SS);
        }
        double rwd=rwd_sum/num_samples_;
        if(rwd>rwd_mx){
            rwd_mx=rwd;
            i_mx=i;
        }
    }
    return rwd_mx;
}

#endif //SIEVE_PAG_H
