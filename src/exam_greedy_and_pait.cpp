//
// Created by weiwei on 2021/5/30.
//

#include "sieve_pait.h"
#include "iset_generator.h"
#include <gflags/gflags.h>
#include "greedy_pait.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_string(obj, "output.txt", "objective file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(p, 0.6, "probability");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    SievePAIT pait(FLAGS_n,FLAGS_B,FLAGS_eps);
    ISetGenerator isgen(FLAGS_n,FLAGS_p);
    GreedyAlg greedy{FLAGS_n,FLAGS_B};

    SocialAcs social_actions;

    std::ifstream data(FLAGS_stream);
    std::string oneline;
    int x=0;
    while(getline(data,oneline)){
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while(read_str>>item)
            temp.push_back(int(item));
        social_actions.emplace_back(std::make_pair(temp[1],temp[2]),temp[0],temp[3]);
        x++;
        if(x==100)
            break;
    }

    int temp=1;
    std::vector<std::tuple<int,double,double>> rst;
    std::ofstream out(FLAGS_obj);
    double sum=0;
    double ritio=0;
    for(auto &s:social_actions){
        ISet is=isgen.getISet();
        pait.update(s,is);
        double pait_mx=pait.getResult();
        std::cout<<temp<<" ";
        std::cout<<"sieve_pait"<<pait_mx<<"  ";
        int p_ocalls=pait.getOracleCalls();

        pait.clear();

        greedy.update(s,is);
        double greedy_gain=greedy.getResult();

        std::cout<<"greedy:"<<greedy_gain<<std::endl;
        int g_ocalls=greedy.getOracleCalls();

//        std::cout<<" "<<g_ocalls<<std::endl;
        greedy.clear();
        sum+=pait_mx/greedy_gain;
        rst.emplace_back(temp,pait_mx,greedy_gain);
        ritio+=p_ocalls/g_ocalls;

        temp++;
    }
    std::cout<<ritio/temp<<std::endl;
    std::cout<<sum/x<<std::endl;
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "pait_and_greedy_n{}b{}p{}eps{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_p,FLAGS_eps));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}

