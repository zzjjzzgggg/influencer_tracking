//
// Created by zhangwei on 2021/7/1.
//

#include "iset_segment.h"
#include "eval_stream.h"
#include <gflags/gflags.h>
#include "obj/graph_obj_fun.h"

DEFINE_string(dir, "../../result/random", "working directory");
DEFINE_string(stream, "reddit.txt", "input streaming data file name");
DEFINE_string(lifespans, "../../lifespans/lmd{:g}n{}L{}.gz",
              "lifespans template");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 20, "budget");
DEFINE_double(lmd, 0.002, "decaying rate");
DEFINE_int32(L, 100000, "maximum lifetime");
DEFINE_int32(T, 1000, "end time");

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    rngutils::default_rng rng;
    EvalStream eval(FLAGS_L, FLAGS_n);

    std::string lifespan_fnm =
        osutils::join(FLAGS_dir, fmt::format(FLAGS_lifespans, FLAGS_lmd, FLAGS_n,
                                             strutils::prettyNumber(FLAGS_L)));
    auto pin = ioutils::getIOIn(lifespan_fnm);
    std::vector<int> lifespans;
    ioutils::TSVParser ss(FLAGS_stream);

    std::vector<std::tuple<int, double>> rst;
    int t = 0;
    while (ss.next() && (t++ < FLAGS_T)) {
        int u = ss.get<int>(1), v = ss.get<int>(2);
        Action a{u, v};

        lifespans.clear();
        pin->load(lifespans);
        ISetSegments segs(lifespans);

        eval.add(a, segs);
        auto obj_mgr = eval.getObjMgr<GraphObjFun>();
        auto users_set = eval.getNodes();

        eval.next();

        double val;
        std::vector<int> users(users_set.begin(), users_set.end());
        if (users.size() > FLAGS_B) {
            auto samples = rngutils::choose(users, FLAGS_B, rng);
            val = obj_mgr.getVal(samples);
        } else {
            val = obj_mgr.getVal(users);
        }
        rst.emplace_back(t, val);
    }

    std::string ofnm = osutils::join(
        FLAGS_dir,
        "random_lmd{:g}n{}k{}L{}T{}.dat"_format(FLAGS_lmd, FLAGS_n, FLAGS_B,
                                                strutils::prettyNumber(FLAGS_L),
                                                strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
