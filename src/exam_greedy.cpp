//
// Created by zhangwei on 2021/7/1.
//

#include "obj/checkin_obj_fun.h"
//#include "obj/stackexchange_obj_fun.h"
#include "greedy_alg.h"
#include <gflags/gflags.h>
#include "eval_stream.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "reddit.txt", "input streaming data file name");
DEFINE_string(lifespans, "../../lifespans/lmd{:g}n{}L{}.gz",
              "lifespans template");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 20, "budget");
DEFINE_double(lmd, 0.002, "decaying rate");
DEFINE_int32(L, 100000, "maximum lifetime");
DEFINE_int32(T, 100, "end time");

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    /*** next line code is used for stackoverflow data ***/
//    EvalStream<StackExObjFun> eval(FLAGS_L);
    /*** next line code is used for check in data ***/
    EvalStream<CheckinObjFun> eval(FLAGS_L);

    std::string lifespan_fnm =
        osutils::join(FLAGS_dir, fmt::format(FLAGS_lifespans, FLAGS_lmd, FLAGS_n,
                                             strutils::prettyNumber(FLAGS_L)));
    auto pin = ioutils::getIOIn(lifespan_fnm);
    std::vector<int> lifespans;
    ioutils::TSVParser ss(FLAGS_stream);

    std::vector<std::tuple<int, double, int>> rst;
    std::vector<std::tuple<int,int>> user_num;
    int t = 0, ocalls = 0;
    /*** notes: we need change some code for different data ***/
    while (ss.next()) {
        int c = ss.get<int>(0), u = ss.get<int>(1), v = ss.get<int>(2);
        Action a{u, v, c, ++t};

        lifespans.clear();
        pin->load(lifespans);
        ISetSegments segs(lifespans);

        eval.add(a, segs);
        auto obj = eval.getObjMgr(FLAGS_n);
        GreedyAlg<CheckinObjFun> greedy(&obj, FLAGS_B);

        auto locations = eval.get_locs();

        /*** next line code is used for getting user num ***/
        //user_num.emplace_back(t,users.size());

        double val = greedy.run(locations);
        ocalls += greedy.getOracleCalls();
        eval.next();

        rst.emplace_back(t, val, ocalls);
        if (t == FLAGS_T) break;
    }
    std::string ofnm = osutils::join(
        FLAGS_dir,
        "greedy_lmd{:g}n{}k{}L{}T{}.dat"_format(FLAGS_lmd, FLAGS_n, FLAGS_B,
                                                strutils::prettyNumber(FLAGS_L),
                                                strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTripletVec(rst, ofnm, "{}\t{}\t{}\n");

    /**
     *
     * get user num
     * /
/*    std::string ofnm1=osutils::join(FLAGS_dir,
                                      "greedy_usernum_lmd{:g}n{}k{}L{}T{}.dat"_format(FLAGS_lmd, FLAGS_n, FLAGS_B,
                                                                              strutils::prettyNumber(FLAGS_L),
                                                                              strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTupleVec(user_num, ofnm1, "{}\t{}\n");*/

    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
