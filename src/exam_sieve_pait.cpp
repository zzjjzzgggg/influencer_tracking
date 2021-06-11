//
// Created by weiwei on 2021/5/29.
//

//#include "subgraph.h"
#include "sieve_pait.h"
#include "iset_generator.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_string(obj, "output.txt", "objective file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(p, 0.6, "probability");


int main(int argc, char* argv[]){
    gflags::SetUsageMessage("xxxx");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    SievePAIT pait(FLAGS_n,FLAGS_B,FLAGS_eps);
    ISetGenerator isgen(FLAGS_n,FLAGS_p);

    SocialAcs social_actions;

    //postid postuserid commentuserid timestamp

    std::ifstream data(FLAGS_stream);
    std::string oneline;
    int x = 0;
    while (getline(data, oneline)) {
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while (read_str >> item)
            temp.push_back(int(item));
        social_actions.emplace_back(std::make_pair(temp[1],temp[2]),temp[0],temp[3]);
        x++;
        if(x==500)
            break;
    }
    std::vector<std::tuple<int,double>> rst;
    int temp=1;
    std::ofstream out(FLAGS_obj);
    for(auto &a:social_actions){
        ISet is=isgen.getISet();
        pait.update(a,is);

        double pag_mx=pait.getResult();
        std::cout<<pag_mx<<std::endl;
        pait.clear();
        rst.emplace_back(temp,pag_mx);
    }
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "pait_n{}b{}eps{}p{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_eps,FLAGS_p
                                                 ));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}