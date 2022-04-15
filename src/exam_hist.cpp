//
// Created by zhangwei on 2021/6/6.
//

#include "histit_red.h"
//#include "obj/checkin_obj_fun.h"
#include "obj/graph_obj_fun.h"

#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "stream.gz", "input stream");
DEFINE_string(lifespans, "../lifespans/lmd{:g}n{}L{}.gz", "lifespans template");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.1, "sievepait epsilon");
DEFINE_double(lmd, 0.001, "decaying rate");
DEFINE_int32(L, 10000, "maximum lifetime");
DEFINE_int32(T, 1000, "end time");

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    HistITRED<GraphObjFun> hist_red(FLAGS_n, FLAGS_B, FLAGS_eps, FLAGS_eps);
    std::string lifespan_fnm =
        osutils::join(FLAGS_dir, fmt::format(FLAGS_lifespans, FLAGS_lmd, FLAGS_n,
                                             strutils::prettyNumber(FLAGS_L)));

    auto pin = ioutils::getIOIn(lifespan_fnm);
    if (pin == nullptr) {
        printf("File %s does not exist!\n", lifespan_fnm.c_str());
        std::exit(-1);
    }

    int t = 0, ocalls = 0;
    std::vector<int> lifespans;
    std::vector<std::tuple<int, double, int, int>> rst;

    printf("\t%-12s%-14s%-12s%-12s\n", "step", "val", "costs", "algs");

    ioutils::TSVParser ss(osutils::join(FLAGS_dir, FLAGS_stream));
    while (ss.next() && t++ < FLAGS_T) {
        Action a{ss.get<int>(0), ss.get<int>(1)};

        lifespans.clear();
        pin->load(lifespans);
        ISetSegments segs(lifespans);

        hist_red.feed(a, segs);
        double val = hist_red.getResult();
        ocalls += hist_red.statOracleCalls();
        int num_algs = hist_red.size();
        rst.emplace_back(t, val, ocalls, num_algs);

        printf("\t%-12d%-12.2f%-12d%-12d\r", t, val, ocalls, num_algs);
        if (t % 1000 == 0) printf("\n");
        fflush(stdout);

        hist_red.reduce();
        hist_red.next();
    }
    printf("\n");

    std::string ofnm =
        osutils::join(FLAGS_dir, "hist_n{}k{}eps{:g}lmd{:g}L{}T{}.dat"_format(
                                     FLAGS_n, FLAGS_B, FLAGS_eps, FLAGS_lmd,
                                     strutils::prettyNumber(FLAGS_L),
                                     strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{:.2f}\t{}\t{}\n");

    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
