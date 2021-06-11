//
// Created by weiwei on 2021/5/30.
//

#ifndef INFLUENCERS_TRACKING_GREEDY_PAIT_H
#define INFLUENCERS_TRACKING_GREEDY_PAIT_H

#include "stdafx.h"
#include "social_influence.h"

class GreedyAlg{
public:
    int budget_;     //k
    int num_samples_;   //n
    SocialInfluence sg_;
    std::vector<SocialInfluence*> sam_graphs_;

public:
    GreedyAlg(const int num_samples,const int budget);
    void update(const SocialAc &a, const ISet& is);
    double getResult();
    void clear(const bool deep=false){
        sg_.clear(deep);
        for(int i=0;i<num_samples_;i++){
            sam_graphs_[i]->clear(deep);
        }
    }
};
GreedyAlg::GreedyAlg(const int num_samples,const int budget)
{
    num_samples_=num_samples;
    budget_=budget;
    for(int i=0;i<num_samples;i++){
        SocialInfluence *sg=new SocialInfluence();
        sam_graphs_.push_back(sg);
    }
}

void GreedyAlg::update(const SocialAc &a, const ISet &is) {
    //add social action
    sg_.addSocialAction(std::get<0>(a).first, std::get<0>(a).second, std::get<1>(a));

    for(auto i:is){
        sam_graphs_[i]->addSocialAction(std::get<0>(a).first, std::get<0>(a).second, std::get<1>(a));
    }
}

double GreedyAlg::getResult(){
    //get nodes
    std::vector<int> nodes=sg_.getNodes();

    double rwd_mx=0;
    std::vector<int> S;
    int mx_gain_node;
    for(int i=0;i<budget_;i++){
        double gain_mx=-100;
        for(auto &node:nodes){
            double gain_sums=0;
            for(int k=0;k<num_samples_;k++){
                gain_sums+=sam_graphs_[k]->getGain(node,S);
            }
            double gain=gain_sums/num_samples_;
            if(gain>gain_mx){
                gain_mx=gain;
                mx_gain_node=node;
            }
        }
        S.push_back(mx_gain_node);
        nodes.erase(remove(nodes.begin(), nodes.end(), mx_gain_node), nodes.end());
        if(nodes.empty()){
            break;
        }
    }
    double gains_all=0;

    for(int i=0;i<num_samples_;i++){
        gains_all+=sam_graphs_[i]->getReward(S);
    }
    rwd_mx=gains_all/num_samples_;
    return  rwd_mx;
}


#endif //INFLUENCERS_TRACKING_GREEDY_PAIT_H
