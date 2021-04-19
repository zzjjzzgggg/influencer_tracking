//
// Created by weiwei on 2021/4/18.
//

#include "sieve_pag.h"
#include "dyn_dgraph_mgr.h"
void SievePAG::addTheta(const int i) {
    /*int pos;
    pos=candidate_buf_.size();
    candidate_buf_.emplace_back()*/

}

void SievePAG::delTheta(const int i) {
    //get pos
    int pos=thi_pos_[i];
    candidate_buf_[pos].clear();
    this_pos_.erase(i);
}

void SievePAG::updateThresholds(std::vector<int> new_thresholds) {
    if(!this_pos_.empty()){
        std::vector<int> v_intersection;
        std::set_intersection(thresholds_.begin(), thresholds_.end(),
                              new_thresholds.begin(), new_thresholds.end(),
                              std::back_inserter(v_intersection));
        std::vector<int> insec1;
        // new theta中有，old theta中没有
        set_difference(thresholds_.begin(), thresholds_.end(), v_intersection.begin(), v_intersection.end(), inserter(insec1, insec1.begin()));

        // old theta中有，new theta中没有
        std::vector<int> insec2;
        set_difference(new_thresholds.begin(), new_thresholds.end(), v_intersection.begin(), v_intersection.end(), inserter(insec2, insec2.begin()));
        for(auto& item:insec1){
            /***
             * clear every stheat
             */
        }
        for(auto& item:insec2){
            /***
             * delete S theta
             */
        }
    }
}
void SievePAG::getGraphEdge(){
    DynDGraphMgr dynDGraphMgr;
    for(auto& social_action:social_actions_){
        dynDGraphMgr.addEdge(social_action.edge.first,social_action.edge.second);
    }
}

double SievePAG::getMaxDelta(std::vector<int> nodes) {
    double maxDelta=0.0;
    /***
     *  calculate Ft(v) get max
     */
    return maxDelta;
}

//get nodes whose influence spread changes
std::vector<int> SievePAG::getInfluencers(SocialAction e) {
    DynDGraphMgr dynDGraphMgr;
    dynDGraphMgr.addEdge(e.edge.first,e.edge.second);
    std::vector<int> affect_nodes=dynDGraphMgr.getAffectedNodes();
    return affect_nodes;
}

void SievePAG::filterNodes(std::vector<int> affect_nodes) {
    for(auto& nodev:affect_nodes){
        for(auto& theta:thresholds_){
            int index=thresholds_.at(theta);
            if(getCandidate(index).size()<budget_&&getGain(affect_nodes)>=theta){
                /***
                 * nodev is added to S theta
                 */
            }
        }
    }
}

//delta S theta  v for filter nodes
double SievePAG::getGain(std::vector<int> affect_nodes) {
    double gain=0.0;

    /***
     * calculate the gain for nodev
     */

    return gain;
}

