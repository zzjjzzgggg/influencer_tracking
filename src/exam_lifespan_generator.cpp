//
// Created by weiwei on 2021/5/5.
//

#include "lifespan_generator.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "../../lifespans", "working directory");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(L, 100, "maximum lifetime");
DEFINE_int32(T, 10000, "stream end time");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_bool(echo, false, "echo");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    LifespanGenerator lifegen(FLAGS_L,1-exp(-FLAGS_lmd));

    auto filename = osutils::join(
            FLAGS_dir, "lmd{:g}n{}L{}.gz"_format(FLAGS_lmd, FLAGS_n,
                                                 strutils::prettyNumber(FLAGS_L)));
    auto pout = ioutils::getIOOut(filename);
    int t = 0;
    while (t++ < FLAGS_T) {
        auto lifespans = lifegen.getLifespans(FLAGS_n);
        pout->save(lifespans);
        if (FLAGS_echo) ioutils::printVec(lifespans);
    }

    printf("Saved to %s.\n", filename.c_str());
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}