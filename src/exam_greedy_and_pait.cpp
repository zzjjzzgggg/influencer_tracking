//
// Created by zhangwei on 2021/5/30.
//

#include "simple_greedy.h"
#include "sieve_pait.h"
#include "iset_generator.h"
#include <gflags/gflags.h>
#include "obj/graph_obj_fun.h"
#include "obj/obj_mgr.h"

DEFINE_string(dir, "../../result/greedy_and_pait", "working directory");
DEFINE_string(stream, "reddit.txt", "input streaming data file name");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 20, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(p, 0.6, "probability");
DEFINE_int32(T,100,"end time");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    SievePAIT<GraphObjFun> pait(FLAGS_n,FLAGS_B,FLAGS_eps);
    ObjMgr<GraphObjFun> obj(FLAGS_n);
    SimpleGreedy<GraphObjFun> greedy(&obj, FLAGS_B);
    ISetGenerator isgen(FLAGS_n,FLAGS_p);

    ioutils::TSVParser ss(FLAGS_stream);
    int t=0,greedy_ora=0,pait_ora=0;
    std::vector<std::tuple<int,double,int,double,int>> rst;
    std::unordered_set<int> users;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2);
        Action a{u,v,c,t};

        ISet iset=isgen.getISet();
        pait.update(a,iset);

        double pait_val=pait.getResult();
        pait_ora+=pait.getOracleCalls();

        pait.clear();

        obj.update(a,iset);
        if(users.find(u)==users.end())
            users.insert(u);
        if(users.find(v)==users.end())
            users.insert(v);

        double greedy_val=greedy.run(users);
        greedy_ora+=greedy.getOracleCalls();
        obj.clear();

        rst.emplace_back(t,pait_val,pait_ora,greedy_val,greedy_ora);
        if(t==FLAGS_T) break;
    }
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "pait_and_greedy_n{}b{}p{}eps{}T{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_p,
                    FLAGS_eps, strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\t{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}

