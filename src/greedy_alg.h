//
// Created by weiwei on 2021/4/29.
//

#ifndef INFLUENCERS_TRACKING_GREEDY_ALG_H
#define INFLUENCERS_TRACKING_GREEDY_ALG_H

#include "dyn_dgraph_mgr_v2.h"

template<class InputMgr>
class GreedyAlg{
private:
    int budget_;     //k
    int num_samples_;   //n
    InputMgr input_mgr_;
    std::vector<InputMgr*> sam_graphs_;  //store the n sample graphs
public:
    GreedyAlg(const int num_samples,const int budget);
    std::vector<int> update(const SocialAc &s,const BernoulliSet& bs);
    double getResult(std::vector<int> nodes);
};
template<class InputMgr>
GreedyAlg<InputMgr>::GreedyAlg(const int num_samples,const int budget)
{
    num_samples_=num_samples;
    budget_=budget;
    for(int i=0;i<num_samples;i++){
        InputMgr *sg=new InputMgr();
        sam_graphs_.push_back(sg);
    }
}

template<class InputMgr>
std::vector<int> GreedyAlg<InputMgr>::update(const SocialAc &s, const ISet &is) {
    //add edge
    input_mgr_.addEdge(s.first.first,s.first.second);

    //get affected nodes
    std::vector<int> nodes=input_mgr_.getAffectedNodes();

    for(auto i:is){
        sam_graphs_[i]->addEdge(s.first.first,s.first.second);
    }

    return nodes;
}

template<class InputMgr>
double GreedyAlg<InputMgr>::getResult(std::vector<int> nodes){
    double rwd_mx=0;
    std::vector<int> S;
    std::vector<int> resultS;
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
//    std::cout<<S.size();
    rwd_mx=gains_all/num_samples_;
    return  rwd_mx;
}

#endif //INFLUENCERS_TRACKING_GREEDY_ALG_H
