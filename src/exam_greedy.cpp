
//
// Created by weiwei on 2021/4/29.
//

#include "greedy_pait.h"
#include "iset_generator.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_string(obj, "output.txt", "objective file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(p, 0.6, "probability");

int main(){
    GreedyAlg greedy{FLAGS_n,FLAGS_B};
    ISetGenerator isgen(FLAGS_n,FLAGS_p);

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
        if(x==100)
            break;
    }

    int temp=1;
    std::vector<std::tuple<int,double>> rst;
    std::ofstream out(FLAGS_obj);
    for(auto &a:social_actions){
        ISet is=isgen.getISet();
        greedy.update(a,is);

        double greedy_gain=greedy.getResult();
        int g_ocalls=greedy.getOracleCalls();

        std::cout<<"greedy:"<<greedy_gain<<std::endl;
        std::cout<<g_ocalls<<std::endl;
        greedy.clear();
        rst.emplace_back(temp,greedy_gain);
        temp++;
    }
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "greedy_n{}b{}p{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_p));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}

