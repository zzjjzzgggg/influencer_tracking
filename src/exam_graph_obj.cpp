//
// Created by zhangwei on 2022/3/22.
//

#include "obj/graph_obj_fun.h"
#include <gflags/gflags.h>
#include "iset_generator.h"
#include "sieve_pait.h"
#include "simple_greedy.h"
#include "obj/obj_mgr.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "reddit.txt", "input streaming data file name");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 20, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(p, 0.6, "probability");
DEFINE_int32(T,100,"end time");


int main(int argc,char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    SievePAIT<GraphObjFun,TAction> sieve(FLAGS_n, FLAGS_B, FLAGS_eps);
    ObjMgr<GraphObjFun,TAction> obj(FLAGS_n);
    SimpleGreedy<GraphObjFun,TAction> greedy(&obj, FLAGS_B);
    ISetGenerator isgen(FLAGS_n, FLAGS_p);

    ioutils::TSVParser ss(FLAGS_stream);

    int t=0,greedy_ora=0,pait_ora=0;
    GraphObjFun graphObjFun;
    std::vector<std::tuple<int,double,int,double,int>> rst;
    std::unordered_set<int> users;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v1=ss.get<int>(2),v2=ss.get<int>(3);
        TAction a{u,v1,v2,c,t};

        ISet iset = isgen.getISet();
        sieve.update(a, iset);

        double pait_val = sieve.getResult();
        pait_ora+= sieve.getOracleCalls();

        sieve.clear();
        obj.update(a,iset);
        if(users.find(u)==users.end())
            users.insert(u);
        if(users.find(v1)==users.end())
            users.insert(v1);
        if(users.find(v2)==users.end())
            users.insert(v2);

        double greedy_val=greedy.run(users);
        greedy_ora+=greedy.getOracleCalls();
        obj.clear();

        rst.emplace_back(t,pait_val,pait_ora,greedy_val,greedy_ora);

        if(t==FLAGS_T) break;
    }

    std::string ofnm = osutils::join(
        FLAGS_dir,
        "test_n{}b{}p{}eps{}T{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_p,
                                                    FLAGS_eps, strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\t{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}