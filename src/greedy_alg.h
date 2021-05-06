//
// Created by weiwei on 2021/4/29.
//

#ifndef INFLUENCERS_TRACKING_GREEDY_ALG_H
#define INFLUENCERS_TRACKING_GREEDY_ALG_H

#include "dyn_dgraph_mgr_v2.h"

class GreedyAlg{
private:
    int budget_;     //k
    int num_samples_;   //n
    DynDGraphMgr input_mgr_;
    std::vector<DynDGraphMgr*> sam_graphs_;  //store the n sample graphs
public:
    GreedyAlg(const int num_samples,const int budget):num_samples_(num_samples),budget_(budget){
        for(int i=0;i<num_samples;i++) {
            DynDGraphMgr *sg = new DynDGraphMgr();
            sam_graphs_.push_back(sg);
        }
    }
    std::vector<int> update(const SocialAc &s,const BernoulliSet& bs);
//    std::vector<int> getResult(std::vector<int> nodes);
    double getResult(std::vector<int> nodes);
};

std::vector<int> GreedyAlg::update(const SocialAc &s, const BernoulliSet &bs) {
    //add edge
    input_mgr_.addEdge(s.first.first,s.first.second);

    //get affected nodes
    std::vector<int> nodes=input_mgr_.getAffectedNodes();

    for(auto b:bs){
        sam_graphs_[b]->addEdge(s.first.first,s.first.second);
    }
//    for(int i=0;i<num_samples_;i++){
//        sam_graphs_[i]->getAffectedNodes();
//    }
    return nodes;
}

double GreedyAlg::getResult(std::vector<int> nodes){
    double rwd_mx=0;
    std::vector<int> S;
    std::vector<int> resultS;
    int mx_gain_node;
    for(int i=0;i<budget_;i++){
        double gain_mx=-100;
        for(auto &node:nodes){
            double gain_sums=0;
            for(int k=0;k<num_samples_;k++){
//                std::vector<int> all_node = sam_graphs_[k]->getNodes();
                std::vector<int> new_S;
//                if(!sam_graphs_[k]->existsNode(node)){
//                    continue;
//                }
                for (auto &item:S) {
//                    bool flag = (std::find(all_node.begin(), all_node.end(), item) != all_node.end());
                    if (sam_graphs_[k]->existsNode(item)) {
                        new_S.push_back(item);
                    }
                }
                if(new_S.empty()){
                    if(sam_graphs_[k]->existsNode(node)){
                        gain_sums+=sam_graphs_[k]->getReward(node);
                    }
                }else{
                    gain_sums+=sam_graphs_[k]->getGain(node,new_S);
                }
                new_S.clear();
            }
            double gain=gain_sums/num_samples_;
            if(gain>gain_mx){
                gain_mx=gain;
                mx_gain_node=node;
            }
        }
        S.push_back(mx_gain_node);
        resultS.push_back(mx_gain_node);
        nodes.erase(remove(nodes.begin(), nodes.end(), mx_gain_node), nodes.end());
//        std::cout<<"node size"<<nodes.size()<<std::endl;
        if(nodes.empty()){
            break;
        }
    }
    double gains_all=0;
    for(int i=0;i<num_samples_;i++){
//        std::vector<int> all_node = sam_graphs_[i]->getNodes();
        std::vector<int> filtered_result;
        for (auto &item:resultS) {
//            bool flag = (std::find(all_node.begin(), all_node.end(), item) != all_node.end());
            if (sam_graphs_[i]->existsNode(item)) {
                filtered_result.push_back(item);
            }
        }
        gains_all+=sam_graphs_[i]->getReward(filtered_result);
    }
    std::cout<<resultS.size();
    rwd_mx=gains_all/num_samples_;
    return  rwd_mx;
}

#endif //INFLUENCERS_TRACKING_GREEDY_ALG_H
