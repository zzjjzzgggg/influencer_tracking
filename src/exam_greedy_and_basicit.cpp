//
// Created by weiwei on 2021/6/3.
//

#include "greedy_basic.h"
#include "lifespan_generator.h"
#include "basic_it.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_string(obj, "output.txt", "objective file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(lmd, .05, "decaying rate");
DEFINE_int32(L, 50, "maximum lifetime");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    GreedyBasic greedy(FLAGS_L,FLAGS_B,FLAGS_n);
    BasicIT basic(FLAGS_L,FLAGS_B,FLAGS_eps,FLAGS_n);
    LifespanGenerator lifegen(FLAGS_L,1-exp(-FLAGS_lmd));

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
    int temp=1;
    std::vector<std::tuple<int,double,double>> rst;
    double ritio_sums=0;
    int ocalls_ritio_sum=0;
    double ocalls_ritio;
    for(auto &s:social_actions){
        std::vector<int> lifespan=lifegen.getLifespans(FLAGS_n);
        ISetSegments segs(lifespan);
        greedy.update(s,segs);
        std::cout<<temp<<" ";
        double greedy_basic=greedy.getResult();
        std::cout<<"greedy_basic "<<greedy_basic<<" ";
        int g_ocalls=greedy.statOracleCalls();
        greedy.next();
//        std::cout<<g_ocalls<<" ";
        basic.update(s,segs);

        double basic_it=basic.getResult();
        std::cout<<"basic_it "<<basic_it<<std::endl;
        int b_ocalls=basic.statOracleCalls();

        basic.next();
        ritio_sums+=basic_it/greedy_basic;

        rst.emplace_back(temp,basic_it,greedy_basic);

        temp++;

//        std::cout<<b_ocalls<<std::endl;
    }
    double ritio=ritio_sums/x;
    std::cout<<ritio;

    std::string ofnm = osutils::join(
            FLAGS_dir,
            "basic_and_greedy_n{}b{}eps{}lmd{}L{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_eps,FLAGS_lmd,FLAGS_L));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}
