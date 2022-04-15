//
// Created by zhangwei on 2021/7/1.
//

#include "obj/graph_obj_fun.h"
//#include "obj/checkin_obj_fun.h"
#include "greedy_alg.h"
#include "eval_stream.h"

#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "stream.gz", "input stream");
DEFINE_string(lifespans, "../lifespans/lmd{:g}n{}L{}.gz", "lifespans template");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(lmd, 0.001, "decaying rate");
DEFINE_int32(L, 10000, "maximum lifetime");
DEFINE_int32(T, 1000, "end time");

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    /*** next line code is used for check in data ***/
    //    EvalStream<CheckinObjFun> eval(FLAGS_L);

    EvalStream eval(FLAGS_L, FLAGS_n);

    std::string lifespan_fnm =
        osutils::join(FLAGS_dir, fmt::format(FLAGS_lifespans, FLAGS_lmd, FLAGS_n,
                                             strutils::prettyNumber(FLAGS_L)));
    auto pin = ioutils::getIOIn(lifespan_fnm);
    if (pin == nullptr) {
        printf("File %s does not exist!\n", lifespan_fnm.c_str());
        std::exit(-1);
    }

    std::vector<int> lifespans;
    ioutils::TSVParser ss(osutils::join(FLAGS_dir, FLAGS_stream));

    std::vector<std::tuple<int, double, int>> rst;
    // std::vector<std::tuple<int, int>> user_num;
    int t = 0, ocalls = 0;
    /*** notes: we need change some code for different data ***/
    while (ss.next() && t++ < FLAGS_T) {
        Action a{ss.get<int>(0), ss.get<int>(1)};

        lifespans.clear();
        pin->load(lifespans);
        ISetSegments segs(lifespans);

        eval.add(a, segs);
        auto obj = eval.getObjMgr<GraphObjFun>();
        GreedyAlg<GraphObjFun> greedy(&obj, FLAGS_B);

        auto users = eval.getNodes();
        //        auto users=eval.getNodes(false);
        /*** next line code is used for getting user num ***/
        // user_num.emplace_back(t, users.size());

        double val = greedy.run(users);
        ocalls += greedy.getOracleCalls();
        rst.emplace_back(t, val, ocalls);

        printf("\t%-12d%-12.2f%-12d%-12lu\r", t, val, ocalls, users.size());
        fflush(stdout);

        eval.next();
    }
    printf("\n");

    std::string ofnm = osutils::join(
        FLAGS_dir,
        "greedy_lmd{:g}n{}k{}L{}T{}.dat"_format(FLAGS_lmd, FLAGS_n, FLAGS_B,
                                                strutils::prettyNumber(FLAGS_L),
                                                strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTripletVec(rst, ofnm, "{}\t{:.2f}\t{}\n");

    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
