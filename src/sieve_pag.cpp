//
// Created by weiwei on 2021/4/18.
//
#include "sieve_pag.h"
/**
 *add threshold theta
 */
void SievePAG::addTheta(const int i) {
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
void SievePAG::delTheta(const int i) {
    //get pos
    int pos=this_pos_[i];
    candidate_buf_[pos].clear();
    recycle_bin_.push(pos);
    this_pos_.erase(i);
}

/**
 * update thresholds
 */
void SievePAG::updateThresholds() {
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
bool SievePAG::updateMaxGain(const std::vector<int> &nodes) {

    bool is_changed=false;
    for(auto u:nodes){ //for all node,calculate the Ft({v})
        double reward_sums=0;
        for(int i=0;i<num_samples_;i++){
//            std::vector<int> all_node=sam_graphs_[i]->getNodes();
//            bool flag=(std::find(all_node.begin(),all_node.end(),u)!=all_node.end());
            if(sam_graphs_[i]->existsNode(u)){
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
void SievePAG::update(const SocialAc &s,const BernoulliSet& bs){
    //add edge
    input_mgr_.addEdge(s.first.first,s.first.second);
    //get affected nodes
    std::vector<int> nodes=input_mgr_.getAffectedNodes();

    for(auto b:bs){
        sam_graphs_[b]->addEdge(s.first.first,s.first.second);
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
//                    std::vector<int> all_node=sam_graphs_[k]->getNodes();
//
//                    if(std::find(all_node.begin(),all_node.end(),u)==all_node.end()){
//                        continue;//u is not in the sample graph
//                    }
//                    std::vector<int> new_S;//the new_S delete the item not in graph
//                    for(auto &item:SS){
//                        bool flag=(std::find(all_node.begin(),all_node.end(),item)!=all_node.end());
//                        if(flag){
//                            new_S.push_back(item);
//                        }
//                    }
//                    gain_sums+=sam_graphs_[k]->getGain(u,new_S);
//                    new_S.clear();

                    if(!sam_graphs_[k]->existsNode(u)){
                        continue;//u is not in the sample graph
                    }
                    std::vector<int> new_S;//the new_S delete the item not in graph
                    for(auto &item:SS){
                        if(sam_graphs_[k]->existsNode(item)){
                            new_S.push_back(item);
                        }
                    }
                    gain_sums+=sam_graphs_[k]->getGain(u,new_S);
                }
                double gain=gain_sums/num_samples_;
                if(gain>=threshold){
                    ca.insert(u);
                }
            }
        }
    }
};

double SievePAG::getResult() {
    int i_mx=-100;//theta-index
    double rwd_mx=0;
    for(auto &pr:this_pos_){
        double rwd_sum=0;
        int i=pr.first;//theta index
        auto ca=getCandidate(i);//get the theta to index
        std::vector<int> SS=ca.getMembers();
        for(int k=0;k<num_samples_;k++) {
//            std::vector<int> all_node = sam_graphs_[k]->getNodes();
            std::vector<int> new_S;
            for (auto &item:SS) {
//                bool flag = (std::find(all_node.begin(), all_node.end(), item) != all_node.end());
                if (sam_graphs_[k]->existsNode(item)) {
                    new_S.push_back(item);
                }
            }
            rwd_sum += sam_graphs_[k]->getReward(new_S);
        }
        double rwd=rwd_sum/num_samples_;
        if(rwd>rwd_mx){
            rwd_mx=rwd;
            i_mx=i;
        }
    }
//    std::vector<int> x=getCandidate(i_mx).getMembers();
//    for(auto &t:x){
//        std::cout<<t<<" ";
//    }
//    std::cout<<getCandidate(i_mx).getMembers().size()<<std::endl;

    return rwd_mx;
}
