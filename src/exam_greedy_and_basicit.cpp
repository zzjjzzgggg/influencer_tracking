//
// Created by weiwei on 2021/6/3.
//

#include "greedy_alg.h"
#include "basic_it.h"
#include "iset_segment.h"
#include "stackexchange_obj_fun.h"
#include <gflags/gflags.h>
#include "eval_stream.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "stackexchange.txt", "input streaming data file name");
DEFINE_string(lifespans, "../../lifespans/lmd{:g}n{}L{}.gz", "lifespans template");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(L, 100, "maximum lifetime");
DEFINE_int32(T,1000,"end time");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    EvalStream<StackExObjFun> eval(FLAGS_L);
    BasicIT<StackExObjFun> basic(FLAGS_L,FLAGS_B,FLAGS_eps,FLAGS_n);

    std::string lifespan_fnm =
            osutils::join(FLAGS_dir, fmt::format(FLAGS_lifespans, FLAGS_lmd, FLAGS_n,
                                                 strutils::prettyNumber(FLAGS_L)));
    auto pin = ioutils::getIOIn(lifespan_fnm);
    std::vector<int> lifespans;

    ioutils::TSVParser ss(FLAGS_stream);
    std::vector<std::tuple<int,double,double>> rst;
    int t=0;
    double ritio_sums=0;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2);
        Action a{u,v,c,t};

        lifespans.clear();
        pin->load(lifespans);
        ISetSegments segs(lifespans);

        eval.add(a,segs);
        auto obj= eval.getObjMgr(FLAGS_n);
        GreedyAlg<StackExObjFun> greedy(&obj, FLAGS_B);
        auto actions=eval.get_Actions();
        auto pop=eval.getPop();
        double greedy_val=greedy.run(pop,actions);
        eval.next();

        basic.update(a,segs);

        double basic_val=basic.getResult();
        std::cout << "basic_val " << basic_val << std::endl;

        basic.next();
        ritio_sums+= basic_val / greedy_val;

        rst.emplace_back(t, basic_val, greedy_val);

        if(t==FLAGS_T) break;
    }
    double ritio=ritio_sums/t;
    std::cout<<ritio;

    std::string ofnm = osutils::join(
            FLAGS_dir,
            "basic_and_greedy_n{}b{}eps{}lmd{}L{}T{}.dat"_format(FLAGS_n, FLAGS_B,
                    FLAGS_eps,FLAGS_lmd,strutils::prettyNumber(FLAGS_L),strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTripletVec(rst, ofnm, "{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
