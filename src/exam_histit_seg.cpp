//
// Created by weiwei on 2021/5/8.
//

#include "histit_seg.h"
#include "lifespan_generator.h"
#include <gflags/gflags.h>
#include "stackexchange_obj_fun.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(L, 500, "maximum lifetime");
DEFINE_int32(end,1000,"end time");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    LifespanGenerator lifegen(FLAGS_L,1-exp(-FLAGS_lmd));
    HistITSEG<StackExObjFun> hist(FLAGS_n,FLAGS_B,FLAGS_eps);

    ioutils::TSVParser ss(FLAGS_stream);
    std::vector<std::tuple<int,double>> rst;
    int t=0;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2), ta= ss.get<int>(3);
        Action a{u,v,c,ta};

        std::vector<int> lifespan=lifegen.getLifespans(FLAGS_n);
        ISetSegments segs(lifespan);
        hist.feed(a,segs);
        double val=hist.getResult();
        std::cout<<t<<" ";
        std::cout<<"hist"<<val<<std::endl;

        hist.next();
        rst.emplace_back(t,val);
        if(t==FLAGS_end) break;
    }
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "hist_n{}b{}eps{}lmd{}L{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_eps,FLAGS_lmd,FLAGS_L));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}