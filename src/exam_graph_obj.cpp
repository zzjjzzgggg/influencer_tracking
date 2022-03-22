//
// Created by zhangwei on 2022/3/22.
//

#include "obj/graph_obj_fun.h"
#include <gflags/gflags.h>
#include "iset_generator.h"
#include "sieve_pait.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "stackexchange.txt", "input streaming data file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(p, 0.6, "probability");
DEFINE_int32(T,1000,"end time");


int main(int argc,char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    SievePAIT<GraphObjFun> sieve(FLAGS_n, FLAGS_B, FLAGS_eps);
    ISetGenerator isgen(FLAGS_n, FLAGS_p);

    ioutils::TSVParser ss("stackexchange.txt");

    int t=0,ocalls=0;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2);
        Action a{u,v,c,t};

        ISet iset = isgen.getISet();
        sieve.update(a, iset);

        double val = sieve.getResult();
        ocalls+= sieve.getOracleCalls();
        std::cout<<t<<" "<<val<<" "<<ocalls<<std::endl;

        sieve.clear();
//        rst.emplace_back(t, val,ocalls);
        if(t==FLAGS_T) break;
    }

//    std::string ofnm = osutils::join(
//        FLAGS_dir,
//        "test_n{}b{}eps{}p{}T{}.dat"_format(FLAGS_n, FLAGS_B, FLAGS_eps, FLAGS_p,
//                                            strutils::prettyNumber(FLAGS_T)));
//    ioutils::saveTripletVec(rst, ofnm, "{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}