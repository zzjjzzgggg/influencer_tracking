#include "../stdafx.h"

#include "../obj/graph_obj_fun.h"
#include "dim_stream.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "../../result/dim", "working directory");
DEFINE_string(stream, "", "input stream");
DEFINE_string(lifespans, "../../lifespans/lmd{:g}n{}L{}.gz",
              "lifespans template");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(lmd, 0.002, "decaying rate");
DEFINE_int32(L, 10000, "maximum lifetime");
DEFINE_int32(T, 10000, "end time");
DEFINE_int32(beta, 32, "beta");

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    DIMStream dim(FLAGS_L, FLAGS_n, FLAGS_beta);

    std::string lifespan_fnm =
        osutils::join(FLAGS_dir, fmt::format(FLAGS_lifespans, FLAGS_lmd, FLAGS_n,
                                             strutils::prettyNumber(FLAGS_L)));
    auto pin = ioutils::getIOIn(lifespan_fnm);
    std::vector<int> lifespans;

    printf("\t%-12s\n", "time");
    int t = 1;
    ioutils::TSVParser ss(FLAGS_stream);
    while (ss.next()) {
        Action a{ss.get<int>(1), ss.get<int>(2)};

        lifespans.clear();
        pin->load(lifespans);
        ISetSegments segs(lifespans);

        dim.add(a, segs);
        dim.update();

        printf("\t%-12d\r", t);
        fflush(stdout);

        if (t == FLAGS_T) break;
        dim.next();
        ++t;
    }
    printf("\n");

    auto seeds = dim.solveIM(FLAGS_B);
    auto obj = dim.getObjMgr<GraphObjFun>();
    double inf = obj.getVal(seeds);
    printf("inf: %.2f\n", inf);

    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
