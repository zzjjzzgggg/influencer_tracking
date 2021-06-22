//
// Created by weiwei on 2021/6/2.
//

#include "basic_it.h"
#include "lifespan_generator.h"
#include <gflags/gflags.h>
#include "stackexchange_obj_fun.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(L, 40, "maximum lifetime");
DEFINE_int32(end,1000,"end time");


int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    BasicIT<StackExObjFun> basic(FLAGS_L,FLAGS_B,FLAGS_eps,FLAGS_n);
    LifespanGenerator lifegen(FLAGS_L,1-exp(-FLAGS_lmd));

    std::vector<std::tuple<int, double>> rst;
    ioutils::TSVParser ss(FLAGS_stream);
    int t=0;
    while (ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2), t= ss.get<int>(3);
        Action a{u,v,c,t};
        std::vector<int> lifespan=lifegen.getLifespans(FLAGS_n);
        ISetSegments segs(lifespan);
        basic.update(a,segs);

        double val=basic.getResult();
        int num_calls=basic.statOracleCalls();

        std::cout<<val<<std::endl;
        //        std::cout<<num_calls<<std::endl;

        basic.next();
        rst.emplace_back(t,val);
        if(t==FLAGS_end) break;
    }
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "basic_n{}b{}eps{}lmd{}L{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_eps,FLAGS_lmd,FLAGS_L));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}



