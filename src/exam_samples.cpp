//
// Created by weiwei on 2021/6/7.
//

#include "social_influence.h"
#include <gflags/gflags.h>
#include "samples.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_int32(n, 2000, "number of max samples");
DEFINE_int32(i,1,"experiment index");
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
    picked_users=rngutils::choose(s_in.users_, FLAGS_k, rng);
    int time=std::get<2>(social_actions.back());

    //the approximate true value
    double fts_true;
    double f=0;
    double repeat_num=100;
    //get mean Ft(S)
    for(int i=0;i<repeat_num;i++){
        //n stream samples
        double fts_sum=0;
        for(int k=0;k<FLAGS_n;k++){
            SocialInfluence *sg=new SocialInfluence();
            for(auto &a:social_actions){
                int u=std::get<0>(a).first;
                int v=std::get<0>(a).second;
                int c=std::get<1>(a);
                int timestamp=std::get<2>(a);
                if(rng.uniform() <= get_prob(FLAGS_lmd,time,timestamp))
                {
                    sg->addSocialAction(u,v,c);
                }
            }
            fts_sum += sg->getReward(picked_users);
            delete sg;
            sg= nullptr;
        }
        f+=fts_sum/FLAGS_n;
    }
    fts_true=f/repeat_num;

    std::cout<<fts_true<<std::endl;

    int sample_size=1000;
    int r=500;
    std::vector<std::tuple<int,double>> rst;
    for(int i=10;i<=sample_size;i+=10){
        std::vector<double> fts_set;
        double s=0;
        for(int j=0;j<r;j++){
            double f_sum=0;
            for(int m=0;m<i;m++) {
                SocialInfluence *sg = new SocialInfluence();
                for(auto &a:social_actions){
                    int u=std::get<0>(a).first;
                    int v=std::get<0>(a).second;
                    int c=std::get<1>(a);
                    int timestamp=std::get<2>(a);
                    if(rng.uniform() <= get_prob(FLAGS_lmd,time,timestamp))
                    {
                        sg->addSocialAction(u,v,c);
                    }
                }
                f_sum+=sg->getReward(picked_users);
                delete sg;
                sg= nullptr;
            }
            //ft_s value
            double f=f_sum/i;
            fts_set.push_back(f);
        }
        double mse= get_mse(fts_set,fts_true);
        double nrmse= sqrt(mse)/fts_true;
        rst.emplace_back(i,nrmse);
    }
//    for(auto item:rst){
//        std::cout<<std::get<0>(item)<<" "<<std::get<1>(item)<<std::endl;
//    }
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "samples_n{}lmd{}k{}i{}.dat"_format(FLAGS_n, FLAGS_lmd,FLAGS_k,FLAGS_i
            ));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}



