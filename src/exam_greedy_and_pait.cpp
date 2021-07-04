//
// Created by weiwei on 2021/5/30.
//

#include "simple_greedy.h"
#include "sieve_pait.h"
#include "iset_generator.h"
#include <gflags/gflags.h>
#include "stackexchange_obj_fun.h"
#include "obj_mgr.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "stackexchange.txt", "input streaming data file name");
DEFINE_int32(n, 50, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(p, 0.6, "probability");
DEFINE_int32(T,1000,"end time");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    SievePAIT<StackExObjFun> pait(FLAGS_n,FLAGS_B,FLAGS_eps);
    ObjMgr<StackExObjFun> obj(FLAGS_n);
    SimpleGreedy<StackExObjFun> greedy(&obj, FLAGS_B);
    ISetGenerator isgen(FLAGS_n,FLAGS_p);

    ioutils::TSVParser ss(FLAGS_stream);
    int t=0;
    std::vector<std::tuple<int,double,double>> rst;
    double sum=0;
    std::unordered_set<int> users;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2);
        Action a{u,v,c,t};

        ISet iset=isgen.getISet();
        pait.update(a,iset);

        double pait_val=pait.getResult();
        std::cout<<t<<" ";
        std::cout<<"sieve_pait"<<pait_val<<"  ";

        pait.clear();

        obj.update(a,iset);
        if(users.find(u)==users.end())
            users.insert(u);
        if(users.find(v)==users.end())
            users.insert(v);
        double greedy_val=greedy.run(users);
        obj.clear();

        std::cout<<"greedy:"<<greedy_val<<std::endl;

        sum+=pait_val/greedy_val;
        rst.emplace_back(t,pait_val,greedy_val);
        if(t==FLAGS_T) break;
    }
    std::cout<<sum/t<<std::endl;
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "pait_and_greedy_n{}b{}p{}eps{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_p,
                    FLAGS_eps, strutils::prettyNumber(FLAGS_T)));
    ioutils::saveTripletVec(rst, ofnm, "{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}

