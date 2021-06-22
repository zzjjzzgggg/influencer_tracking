
//
// Created by weiwei on 2021/4/29.
//

#include "greedy_pait.h"
#include "iset_generator.h"
#include <gflags/gflags.h>
#include "stackexchange_obj_fun.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(p, 0.6, "probability");
DEFINE_int32(end,1000,"end time");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    GreedyAlg<StackExObjFun> greedy{FLAGS_n,FLAGS_B};
    ISetGenerator isgen(FLAGS_n,FLAGS_p);

    ioutils::TSVParser ss(FLAGS_stream);
    std::vector<std::tuple<int,double>> rst;
    int t=0;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2), t= ss.get<int>(3);
        Action a{u,v,c,t};
        ISet iset=isgen.getISet();

        greedy.update(a,iset);
        double val=greedy.getResult();
//        int num_ocalls=greedy.getOracleCalls();

        std::cout<<"greedy:"<<val<<std::endl;
//        std::cout<<num_ocalls<<std::endl;
        greedy.clear();
        rst.emplace_back(t,val);
        if(t==FLAGS_end) break;
    }

    std::string ofnm = osutils::join(
            FLAGS_dir,
            "greedy_n{}b{}p{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_p));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}

