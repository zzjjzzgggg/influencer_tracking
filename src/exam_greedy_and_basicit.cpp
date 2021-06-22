//
// Created by weiwei on 2021/6/3.
//

#include "greedy_basic.h"
#include "basic_it.h"
#include "lifespan_generator.h"
#include "stackexchange_obj_fun.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(lmd, .05, "decaying rate");
DEFINE_int32(L, 50, "maximum lifetime");
DEFINE_int32(end,1000,"end time");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    GreedyBasic<StackExObjFun> greedy(FLAGS_L,FLAGS_B,FLAGS_n);
    BasicIT<StackExObjFun> basic(FLAGS_L,FLAGS_B,FLAGS_eps,FLAGS_n);
    LifespanGenerator lifegen(FLAGS_L,1-exp(-FLAGS_lmd));

    ioutils::TSVParser ss(FLAGS_stream);
    std::vector<std::tuple<int,double,double>> rst;
    int t=0;
    double ritio_sums=0;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2), t= ss.get<int>(3);
        Action a{u,v,c,t};

        std::vector<int> lifespan=lifegen.getLifespans(FLAGS_n);
        ISetSegments segs(lifespan);
        greedy.update(a,segs);
        std::cout<<t<<" ";
        double greedy_val=greedy.getResult();
        std::cout<<"greedy_basic "<<greedy_val<<" ";

        greedy.next();
//        std::cout<<g_ocalls<<" ";
        basic.update(a,segs);

        double basic_val=basic.getResult();
        std::cout << "basic_val " << basic_val << std::endl;

        basic.next();
        ritio_sums+= basic_val / greedy_val;

        rst.emplace_back(t, basic_val, greedy_val);


        if(t==FLAGS_end) break;
    }
    double ritio=ritio_sums/t;
    std::cout<<ritio;

    std::string ofnm = osutils::join(
            FLAGS_dir,
            "basic_and_greedy_n{}b{}eps{}lmd{}L{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_eps,FLAGS_lmd,FLAGS_L));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}
