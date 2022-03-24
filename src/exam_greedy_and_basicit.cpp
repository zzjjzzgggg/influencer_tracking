//
// Created by zhangwei on 2021/6/3.
//

#include "greedy_alg.h"
#include "basic_it.h"
//#include "obj/stackexchange_obj_fun.h"
#include "obj/graph_obj_fun.h"
#include <gflags/gflags.h>
#include "eval_stream.h"

DEFINE_string(dir, "../../result/basicit_and_greedy", "working directory");
DEFINE_string(stream, "test_reddit_comment_tree.txt", "input streaming data file name");
DEFINE_string(lifespans, "../../lifespans/lmd{:g}n{}L{}.gz", "lifespans template");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 20, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(L, 100, "maximum lifetime");
DEFINE_int32(T,100,"end time");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    EvalStream<GraphObjFun,TAction> eval(FLAGS_L);
    BasicIT<GraphObjFun,TAction> basic(FLAGS_L,FLAGS_B,FLAGS_eps,FLAGS_n);

    std::string lifespan_fnm =
            osutils::join(FLAGS_dir, fmt::format(FLAGS_lifespans, FLAGS_lmd, FLAGS_n,
                                                 strutils::prettyNumber(FLAGS_L)));
    auto pin = ioutils::getIOIn(lifespan_fnm);
    std::vector<int> lifespans;

    ioutils::TSVParser ss(FLAGS_stream);
    std::vector<std::tuple<int,double,int,double,int>> rst;
    int t=0,basic_ocalls=0,greedy_ocalls=0;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v1=ss.get<int>(2),v2=ss.get<int>(3);
        TAction a{u,v1,v2,c,t};

        lifespans.clear();
        pin->load(lifespans);
        ISetSegments segs(lifespans);

        eval.add(a,segs);
        auto obj= eval.getObjMgr(FLAGS_n);
        GreedyAlg<GraphObjFun,TAction> greedy(&obj, FLAGS_B);

        auto users=eval.get_users();
        double greedy_val=greedy.run(users);
        greedy_ocalls+=greedy.getOracleCalls();
        eval.next();

        basic.update(a,segs);
        double basic_val=basic.getResult();
        basic_ocalls+=basic.statOracleCalls();
        basic.next();

        rst.emplace_back(t, basic_val, basic_ocalls,greedy_val,greedy_ocalls);
        if(t==FLAGS_T) break;
    }

    std::string ofnm = osutils::join(
            FLAGS_dir,
            "basic_and_greedy_n{}b{}eps{}lmd{}L{}T{}.dat"_format(FLAGS_n, FLAGS_B,
                    FLAGS_eps,FLAGS_lmd,
                    strutils::prettyNumber(FLAGS_L),strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\t{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
