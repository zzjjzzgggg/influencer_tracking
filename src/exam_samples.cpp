//
// Created by weiwei on 2021/6/7.
//

#include "basic_it.h"
#include "iset_segment.h"
#include "lifespan_generator.h"
#include <gflags/gflags.h>
#include "samples.h"

int main(){
    rngutils::default_rng rng;
    int L=40;
    int budget=10;
    double eps=0.2;
    int num_samples=10;

    double lmd=0.01;

    BasicIT basic(L,budget,eps,num_samples);

    LifespanGenerator lifegen(L,1-exp(-lmd));

    std::string filename = "comment_post.txt";
    std::ifstream data(filename);
    std::string oneline;
    int x=0;
    UVCs social_actions;
    while(getline(data,oneline)){
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while(read_str>>item)
            temp.push_back(int(item));
        UVC soca=std::make_pair(std::make_pair(temp[1],temp[2]),temp[0]);
        social_actions.push_back(soca);
        x++;
        if(x==500)
            break;
    }


    int k=10;
    int sample_num=10;

    std::vector<int> users;
    int rpt=20;
//    for(auto &a:social_actions){
//        std::vector<int> lifespan=lifegen.getLifespans(num_samples);
//        ISetSegments segs(lifespan);
//        basic.update(a,segs);
//
//        if(std::find(users.begin(),users.end(),a.first.first)==users.end()){
//            users.push_back(a.first.first);
//        }
//        if(std::find(users.begin(),users.end(),a.first.first)==users.end()){
//            users.push_back(a.first.second);
//        }
//
//        if(++num==batch_size){
//            auto samples = rngutils::choose(users,s_num, rng);
//            for(int r=0;r<rpt;rpt++){
//
//            }
//        }
//
//        basic.next();
//    }

    /*
     * operation
     * */

    //get k users
    std::vector<int> picked_users;
    if(users.size()<=k){
        picked_users.assign(social_actions.begin(), social_actions.end());
    }else{
        picked_users=rngutils::choose(users, sample_num, rng);
    }


    int time=1279638521;

    SocialInfluence sc;

    std::vector<std::vector<UVC>> sample_scs;
    sample_scs.reserve(sample_num);
    std::vector<SocialInfluence> sc_info;
//    sc_info.reserve(sample_num);
    std::vector<double> fs;
    for(int r=0;r<rpt;r++){
        //n times sampling
        double f=0;
        for(int i=0;i<sample_num;i++){
            for(auto &a:social_actions){
                if(rng.uniform() <= get_prob(lmd,time,a.second))
//                    sample_scs[i].push_back(a);
                    sc_info[i].addSocialAction(a.first.first,a.first.second,a.second);
            }
            double value=sc_info[i].getReward(picked_users);
            f+=value;
        }
        fs.push_back(f/sample_num);
    }

    double expect=get_expect();
    double nrmse= cal_nrmse(fs,expect);
    std::cout<<nrmse<<std::endl;
    return 0;
}



