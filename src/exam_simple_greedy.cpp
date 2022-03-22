//
// Created by zhangwei on 2021/7/1.
//

#include "iset_generator.h"
#include "simple_greedy.h"
#include <gflags/gflags.h>
#include "obj/stackexchange_obj_fun.h"
#include "obj/obj_mgr.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "stackexchange.txt", "input streaming data file name");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 20, "budget");
DEFINE_double(p, 0.6, "probability");
DEFINE_int32(T,1000,"end time");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    ObjMgr<StackExObjFun> obj(FLAGS_n);
    SimpleGreedy<StackExObjFun> greedy(&obj, FLAGS_B);
    ISetGenerator isgen(FLAGS_n,FLAGS_p);

    ioutils::TSVParser ss(FLAGS_stream);
    std::vector<std::tuple<int,double,int>> rst;
    int t=0,ocalls = 0;
    std::unordered_set<int> users;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2), t= ss.get<int>(3);
        Action a{u,v,c,t};
        ISet iset=isgen.getISet();

        obj.update(a,iset);
        if(users.find(u)==users.end())
            users.insert(u);
        if(users.find(v)==users.end())
            users.insert(v);
        double val=greedy.run(users);
        ocalls+= greedy.getOracleCalls();
        obj.clear();

        rst.emplace_back(t,val,ocalls);
        if(t==FLAGS_T) break;
    }
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "simple_greedy_n{}b{}p{}T{}.dat"_format(FLAGS_n, FLAGS_B,
                    FLAGS_p,strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTripletVec(rst, ofnm, "{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}


