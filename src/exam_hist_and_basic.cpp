//
// Created by zhangwei on 2021/5/8.
//

#include "histit_seg.h"
#include "basic_it.h"
#include "lifespan_generator.h"
#include "obj/stackexchange_obj_fun.h"

#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "stackexchange.txt", "input streaming data file name");
DEFINE_string(lifespans, "../../lifespans/lmd{:g}n{}L{}.gz", "lifespans template");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 20, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(lmd, 0.002, "decaying rate");
DEFINE_int32(L, 1000, "maximum lifetime");
DEFINE_int32(T, 10, "end time");

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    BasicIT<StackExObjFun> basic(FLAGS_L, FLAGS_B, FLAGS_eps, FLAGS_n);
    HistITSEG<StackExObjFun> hist(FLAGS_n, FLAGS_B, FLAGS_eps);

    std::string lifespan_fnm =
            osutils::join(FLAGS_dir, fmt::format(FLAGS_lifespans, FLAGS_lmd, FLAGS_n,
                                                 strutils::prettyNumber(FLAGS_L)));
    auto pin = ioutils::getIOIn(lifespan_fnm);

    ioutils::TSVParser ss(FLAGS_stream);
    std::vector<int> lifespans;
    std::vector<std::tuple<int, double, double>> rst;
    int t = 0;
    while (ss.next()) {
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v = ss.get<int>(2);
        Action a{u, v, c, t};

        lifespans.clear();
        pin->load(lifespans);
        ISetSegments segs(lifespans);

        hist.feed(a, segs);
        double hist_val = hist.getResult();
        hist.next();

        basic.update(a, segs);
        double basic_val = basic.getResult();
        basic.next();

        rst.emplace_back(t, basic_val, hist_val);
        if (t == FLAGS_T) break;
    }
    std::string ofnm = osutils::join(
        FLAGS_dir, "basic_and_hist_n{}b{}eps{}lmd{}L{}T{}.dat"_format(
                       FLAGS_n, FLAGS_B, FLAGS_eps, FLAGS_lmd,
                       strutils::prettyNumber(FLAGS_L),strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTripletVec(rst, ofnm, "{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
