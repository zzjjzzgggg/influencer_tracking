//
// Created by zhangwei on 2021/5/29.
//

#include "sieve_pait.h"
#include "iset_generator.h"
#include "obj/graph_obj_fun.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "../../result/sieve_pait", "working directory");
DEFINE_string(stream, "test_reddit_comment_tree.txt", "input streaming data file name");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 20, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(p, 0.6, "probability");
DEFINE_int32(T,20,"end time");


int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    SievePAIT<GraphObjFun,TAction> sieve(FLAGS_n, FLAGS_B, FLAGS_eps);
    ISetGenerator isgen(FLAGS_n, FLAGS_p);

    std::vector<std::tuple<int, double,int>> rst;
    ioutils::TSVParser ss(FLAGS_stream);
    int t=0,ocalls=0;
    while (ss.next()) {
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v1=ss.get<int>(2),v2=ss.get<int>(3);
        TAction a{u,v1,v2,c,t};

        ISet iset = isgen.getISet();
        sieve.update(a, iset);

        double val = sieve.getResult();
        ocalls+= sieve.getOracleCalls();

        sieve.clear();
        rst.emplace_back(t, val,ocalls);
        if(t==FLAGS_T) break;
    }

    std::string ofnm = osutils::join(
        FLAGS_dir,
        "pait_n{}b{}eps{}p{}T{}.dat"_format(FLAGS_n, FLAGS_B, FLAGS_eps, FLAGS_p,
                                            strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTripletVec(rst, ofnm, "{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
