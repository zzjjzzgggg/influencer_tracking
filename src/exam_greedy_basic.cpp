//
// Created by weiwei on 2021/5/10.
//

#include "greedy_basic.h"
#include "lifespan_generator.h"
#include <gflags/gflags.h>
#include "stackexchange_obj_fun.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "stackexchange.txt", "input streaming data file name");
DEFINE_string(lifespans, "../../lifespans/lmd{:g}n{}L{}.gz", "lifespans template");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 20, "budget");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(L, 100, "maximum lifetime");
DEFINE_int32(end,1000,"end time");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    GreedyBasic<StackExObjFun> greedy(FLAGS_L,FLAGS_B,FLAGS_n);
    std::string lifespan_fnm =
            osutils::join(FLAGS_dir, fmt::format(FLAGS_lifespans, FLAGS_lmd, FLAGS_n,
                                                 strutils::prettyNumber(FLAGS_L)));
    auto pin = ioutils::getIOIn(lifespan_fnm);
    std::vector<int> lifespans;
    std::vector<std::tuple<int,double,int>> rst;
    ioutils::TSVParser ss(FLAGS_stream);
    int t=0,ocalls=0;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2);
        Action a{u,v,c,t};

        lifespans.clear();
        pin->load(lifespans);
        ISetSegments segs(lifespans);

        greedy.update(a,segs);

        double val=greedy.getResult();
        ocalls+=greedy.statOracleCalls();
        std::cout<<val<<std::endl;

        rst.emplace_back(t,val,ocalls);
        greedy.next();
        if(t==FLAGS_end) break;
    }

    std::string ofnm = osutils::join(
            FLAGS_dir,
            "greedy_basic_n{}b{}lmd{}L{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_lmd,FLAGS_L));
    ioutils::saveTripletVec(rst, ofnm, "{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}