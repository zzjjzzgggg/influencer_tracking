//
// Created by weiwei on 2021/6/7.
//

#include "social_influence.h"
#include "iset_segment.h"
#include <gflags/gflags.h>
#include "samples.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(k,10,"number of picked users");
DEFINE_int32(R, 20, "repeat");

int main(){
    rngutils::default_rng rng;

    std::ifstream data(FLAGS_stream);
    std::string oneline;
    int x=0;
    SocialAcs social_actions;
    while(getline(data,oneline)){
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while(read_str>>item)
            temp.push_back(int(item));
        social_actions.emplace_back(std::make_pair(temp[1],temp[2]),temp[0],temp[3]);
        x++;
        if(x==1000)
            break;
    }

    SocialInfluence s_in;
    for(auto &a:social_actions){
        s_in.addSocialAction(std::get<0>(a).first,std::get<0>(a).second,std::get<1>(a));
    }

    std::vector<int> picked_users;
    picked_users=rngutils::choose(s_in.users, FLAGS_k, rng);
    int time=std::get<2>(social_actions.back());

    std::vector<SocialInfluence*> sc_infl;
    for(int i=0;i<FLAGS_n;i++){
        SocialInfluence *sg=new SocialInfluence();
        sc_infl.push_back(sg);
    }
    std::vector<double> fs;
    std::map<int,int> u_t;
    for(int r=0;r<FLAGS_R;r++){
        //n times sampling
        double f=0;
        for(int i=0;i<FLAGS_n;i++){
            for(auto &a:social_actions){
                int u=std::get<0>(a).first;
                int v=std::get<0>(a).second;
                int timestamp=std::get<2>(a);
                if(std::find(picked_users.begin(),picked_users.end(),u)!=picked_users.end()){
                    u_t[u]= timestamp;
                }
                if(std::find(picked_users.begin(),picked_users.end(),v)!=picked_users.end()){
                    u_t[v]= timestamp;
                }
                if(rng.uniform() <= get_prob(FLAGS_lmd,time,timestamp))
                {
                    sc_infl[i]->addSocialAction(u,v,timestamp);
                }

            }
            double value=sc_infl[i]->getReward(picked_users);
            f+=value;
        }
        for(int i=0;i<FLAGS_n;i++){
            sc_infl[i]->clear(true);
        }
        fs.push_back(f/FLAGS_n);
    }
    for(auto &item:fs){
        std::cout<<item<<std::endl;
    }
    double expect=get_expect(FLAGS_lmd,FLAGS_k,s_in, time,u_t);
    std::cout<<expect<<std::endl;
    double nrmse= cal_nrmse(fs,expect);
    std::cout<<nrmse<<std::endl;

    return 0;
}



