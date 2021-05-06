//
// Created by weiwei on 2021/4/18.
//

#ifndef SIEVEPAG_SIEVE_PAG_H
#define SIEVEPAG_SIEVE_PAG_H

#include "dyn_dgraph_mgr.h"
#include "candidate.h"

class SievePAG{
public:
    int num_samples_;   //n
    int budget_;     //k
    double eps_;  //epsilon
    double mx_gain_; //max_gain

    DynDGraphMgr input_mgr_;
    std::vector<DynDGraphMgr*> sam_graphs_;  //store the n sample graphs

    std::vector<Candidate> candidate_buf_; //store the candidate
    std::map<int,int> this_pos_;//theta_index->buf_position
    std::vector<int> recycle_bin_;  //a vector of positions for recycling

private:
    //theta = (1+\epsilon)^i / (2k)
    inline double getThreshold(const int i)const{
        return std::pow(1+eps_,i)/(2*budget_);
    }
    // Given a threshold index i, return candidate c.
    inline const Candidate& getCandidate(const int i) const {
        return candidate_buf_[this_pos_.at(i)];
    }

    //for a threshold index i,return candidate c
    inline Candidate & getCandidate(const int i) {
        return candidate_buf_[this_pos_.at(i)];
    }

    // Add a new threshold indexed by i
    void addTheta(const int i);
    void delTheta(const int i);

public:
    SievePAG(const int num_samples,const int budget,const double eps):num_samples_(num_samples),budget_(budget),eps_(eps)
    ,mx_gain_(0){
        candidate_buf_.reserve((int)(std::log2(2 * budget_) / eps_));
        sam_graphs_.reserve(num_samples);
        int i;
        for(i=0;i<num_samples;i++){
            DynDGraphMgr *sg=new DynDGraphMgr();
            sam_graphs_.push_back(sg);
        }
    }

    SievePAG& operator=(const SievePAG& s){
        num_samples_=s.num_samples_;
        budget_=s.budget_;
        eps_=s.eps_;
        candidate_buf_=s.candidate_buf_;
        this_pos_=s.this_pos_;
        return *this;
    }


    void clear(const bool deep=false){
        mx_gain_=0;
        std::cout<<mx_gain_;
        this_pos_.clear();
        recycle_bin_.clear();
        for(auto& cad:candidate_buf_){
            cad.clear();
        }

        input_mgr_.clear(deep);
        for(int i=0;i<num_samples_;i++){
            sam_graphs_[i]->clear(deep);
        }
    }

    //Process social action e and its Bernoulli set
    void feed(const SocialAc &s,const BernoulliSet& bs);

    // Get current maximum reward.
    std::vector<int> getResult();

    void updateThresholds();

    bool updateMaxGain(std::vector<int> &nodes);

};

/**
 *add threshold theta
 */
void SievePAG::addTheta(const int i) {
    int pos;
    if(!recycle_bin_.empty()){//if recycle_bin has an unoccupied room
        pos=recycle_bin_.back();//the last
        recycle_bin_.pop_back();//delete the last one in recycle bin
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
void SievePAG::delTheta(const int i) {
    //get pos
    int pos=this_pos_[i];
    candidate_buf_[pos].clear();
    recycle_bin_.push_back(pos);
    this_pos_.erase(i);
}

/**
 * update thresholds
 */
void SievePAG::updateThresholds() {
    int new_li =
            std::floor(std::log((1 - eps_) * mx_gain_) / std::log(1 + eps_)),
            new_ui =
            std::ceil(std::log(2 * budget_ * mx_gain_) / std::log(1 + eps_));
    int li, ui;               // lower bound and upper bound of theata index
    if (!this_pos_.empty()) {  // delete outdated thresholds
        li = this_pos_.begin()->first;//get the theta index
        ui = this_pos_.rbegin()->first;//get the theta index reverse
        while (li <= ui && li < new_li) {
            delTheta(li);//
            li++;
        }
    }
    li = this_pos_.empty() ? new_li : ui + 1;
    for (int i = li; i <= new_ui; i++) addTheta(i);
}

/**
 * check whether the delta change
 */
bool SievePAG::updateMaxGain( std::vector<int> &nodes) {
    bool is_changed=false;
    for(auto &u:nodes){ //for all node,calculate the Ft({v})
        double reward_sums=0;
        for(int i=0;i<num_samples_;i++){
            std::vector<int> all_node=sam_graphs_[i]->getNodes();
            bool flag=(std::find(all_node.begin(),all_node.end(),u)!=all_node.end());
            if(flag){
                reward_sums+=sam_graphs_[i]->getReward(u);
            }
        }
        double reward=reward_sums/num_samples_;
        if(reward>mx_gain_){
            mx_gain_=reward;
            is_changed= true;
        }
    }
    return is_changed;
}

// Process social action s and its Bernoulli set.
void SievePAG::feed(const SocialAc &s,const BernoulliSet& bs){
    //add edge
//    std::cout<<"no"<<std::endl;
    input_mgr_.addEdge(s.first.first,s.first.second);

//    std::cout<<"yes"<<std::endl;
    //get influence nodes
    std::vector<int> nodes=input_mgr_.getAffectedNodes();
//    if(nodes.empty())return;
    for(auto x:nodes){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;
    for(auto b:bs){
        // add the edge  for the graph index in bs
        sam_graphs_[b]->addEdge(s.first.first,s.first.second);
    }
    for(int i=0;i<num_samples_;i++){
        sam_graphs_[i]->getAffectedNodes();
    }
    //if max delta change ,delta change,need to update thresholds
    if(updateMaxGain(nodes)) {
        updateThresholds();
    }

//       filter nodes by thresholds
    for(auto u:nodes){
        for(auto &pr:this_pos_){
            int i=pr.first;//theta-index
//            std::cout<<i<<std::endl;
            auto& ca= getCandidate(i);//definite cite may need to insert item

            if(!ca.isMember(u)&&ca.size()<budget_){
                double threshold=getThreshold(i);
                double gain_sums=0;
                std::vector<int> SS=ca.getMembers();
//                for(auto &s:SS){
//                    std::cout<<s<<std::endl;
//                }
                for(int k=0;k<num_samples_;k++){
                    std::vector<int> all_node=sam_graphs_[k]->getNodes();

                    if(std::find(all_node.begin(),all_node.end(),u)==all_node.end()){
                        continue;//u is not in the sample graph
                    }
                    std::vector<int> new_S;//the new_S delete the item not in graph
                    for(auto &item:SS){
                        bool flag=(std::find(all_node.begin(),all_node.end(),item)!=all_node.end());
                        if(flag){
                            new_S.push_back(item);
                        }
                    }
                    gain_sums+=sam_graphs_[k]->getGain(u,new_S);
                    new_S.clear();
                }
                double gain=gain_sums/num_samples_;
                if(gain>threshold){
                    ca.insert(u);
                }
            }
//            std::vector<int> value=ca.getMembers();
//            for(auto &item:value){
//                std::cout<<"zw"<<item;
//            }
//            std::cout<<std::endl;
         }
    }

};

std::vector<int> SievePAG::getResult(){
    int i_mx=-100;//theta-index
    double rwd_mx=0;
    int temp=0;
    for(auto &pr:this_pos_){
        double rwd_sum=0;
        temp++;
        int i=pr.first;//theta index
        auto ca=getCandidate(i);//get the theta to index
        std::vector<int> SS=ca.getMembers();
//        for(int k=0;k<num_samples_;k++){
//             std::vector<int> all_node=sam_graphs_[k]->getNodes();
//             std::vector<int> new_S;
//             for(auto &item:S){
//                 bool flag=(std::find(all_node.begin(),all_node.end(),item)!=all_node.end());
//                 if(flag){
//                     new_S.push_back(item);
//                 }
//             }
//             rwd_sum+=sam_graphs_[k]->getReward(new_S);
//             new_S.clear();
//        }
//            std::vector<int> all_node=input_mgr_.getNodes();
//            std::vector<int> new_S;
//            for(auto &item:S){
//                bool flag=(std::find(all_node.begin(),all_node.end(),item)!=all_node.end());
//                if(flag){
//                    new_S.push_back(item);
//                }
//            rwd_sum+=input_mgr_.getReward(new_S);
//            new_S.clear();
//        }
        rwd_sum+=input_mgr_.getReward(SS);
        double rwd=rwd_sum/num_samples_;
        if(rwd>rwd_mx){
            rwd_mx=rwd;
            i_mx=i;
        }
    }

    return getCandidate(i_mx).getMembers();
}


#endif //SIEVEPAG_SIEVE_PAG_H
