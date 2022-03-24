//
// Created by zhangwei on 2021/6/6.
//

#include "histit_red.h"
#include <gflags/gflags.h>
//#include "obj/stackexchange_obj_fun.h"
#include "obj/graph_obj_fun.h"

DEFINE_string(dir, "../../result/hist_red", "working directory");
DEFINE_string(stream, "test_reddit_comment_tree.txt", "input streaming data file name");
DEFINE_string(lifespans, "../../lifespans/lmd{:g}n{}L{}.gz", "lifespans template");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 20, "budget");
DEFINE_double(sieve_eps,0.1,"sievepait epsilon");
DEFINE_double(red_eps, 0.1, "histit_red epsilon");
DEFINE_double(lmd, 0.002, "decaying rate");
DEFINE_int32(L, 100000, "maximum lifetime");
DEFINE_int32(T,100,"end time");


int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    HistITRED<GraphObjFun,TAction> hist_red(FLAGS_n,FLAGS_B,FLAGS_red_eps,FLAGS_sieve_eps);
    std::string lifespan_fnm =
            osutils::join(FLAGS_dir, fmt::format(FLAGS_lifespans, FLAGS_lmd, FLAGS_n,
                                                 strutils::prettyNumber(FLAGS_L)));

    auto pin = ioutils::getIOIn(lifespan_fnm);
    std::vector<int> lifespans;
    std::vector<std::tuple<int, double,int,int>> rst;

    ioutils::TSVParser ss(FLAGS_stream);
    int t=0,ocalls=0;
    while (ss.next()) {
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v1=ss.get<int>(2),v2=ss.get<int>(3);
        TAction a{u,v1,v2,c,t};

        lifespans.clear();
        pin->load(lifespans);
        ISetSegments segs(lifespans);

        hist_red.feed(a, segs);
        double val=hist_red.getResult();
        ocalls+=hist_red.statOracleCalls();
        int num_algs=hist_red.size();
        rst.emplace_back(t,val,ocalls,num_algs);

        hist_red.reduce();
        hist_red.next();
        if(t==FLAGS_T) break;
    }
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "histred_n{}b{}sieve_eps{}red_eps{}lmd{:g}L{}T{}.dat"_format(FLAGS_n,
                    FLAGS_B,FLAGS_sieve_eps,FLAGS_red_eps,FLAGS_lmd,
                    strutils::prettyNumber(FLAGS_L),strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
