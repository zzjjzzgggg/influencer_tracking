//
// Created by weiwei on 2021/6/7.
//

#include "social_influence.h"
#include "iset_segment.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "test.txt", "input streaming data file name");
DEFINE_int32(n, 2000, "number of samples");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(k,10,"number of picked users");

int main(int argc, char* argv[]){
    rngutils::default_rng rng;
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

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
        for(auto &a:social_actions){
            int u=std::get<0>(a).first;
            int v=std::get<0>(a).second;
            int c=std::get<1>(a);
            int timestamp=std::get<2>(a);
            if(rng.uniform() <= get_prob(FLAGS_lmd,time,timestamp))
            {
                sc_infl[i]->addSocialAction(u,v,c);
            }
        }
    }
    int temp=1;
    std::vector<std::tuple<int,double>> rst;
    for(int k=1;k<=FLAGS_n;k+=1){
        double f=0;
        for(int i=0;i<k;i++){
            double value=sc_infl[i]->getReward(picked_users);
            f+=value;
        }
        rst.emplace_back(temp,f/k);
        temp++;
    }

    std::string ofnm = osutils::join(
            FLAGS_dir,
            "samples_n{}lmd{}k{}.dat"_format(FLAGS_n, FLAGS_lmd,FLAGS_k
            ));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}



