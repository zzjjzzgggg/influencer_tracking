#include <iostream>
#include "social_stream.h"

int main() {

    /***
     *
     * a test
     */
    Edge x={1,1};
    std::vector<Edge> edge;
    edge.push_back(x);
    int budget_k;
    double epsilon;
    BernoulliSetGenerator bernoulliSetGenerator(100,47);
    std::vector<SocialAction> social_actions;
    SocialStream socialstream(social_actions);
    int delta=0;
    std::vector<double> thresholds;
    thresholds.push_back(1);
    std::unordered_map<int,std::vector<int>> candidate_sets;
    candidate_sets.insert(std::pair<int,std::vector<int>>(1,{1,1}));

    for(auto &s:socialstream.social_actions){

        auto bSet=bernoulliSetGenerator.getBernoulliSet();
        std::vector<int> nodes;

        for(auto &node_v:nodes){
            for(auto &theta:thresholds){
                //int gain=nodev.getgain(candidta_sets[theta])
                if(candidate_sets[theta].size()<budget_k){
                    candidate_sets[delta].push_back(node_v);
                }
            }
        }
        //假设现在get_value;
        std::unordered_map<int,std::vector<int>>::iterator iter;
        //auto first_item=candidate_sets[0];
        for(iter=candidate_sets.begin();iter!=candidate_sets.end();iter++){
            //get the max Ft;
            //St=
        }

    }
    return 0;
}
