//
// Created by zhangwei on 2022/3/22.
//

#include "obj/graph_obj_fun.h"
#include <gflags/gflags.h>

int main(int argc,char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    GraphObjFun graphObjFun;

    ioutils::TSVParser ss("test.txt");

    int t=0;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2);
        Action a{u,v,c,t};

        graphObjFun.add(a);


    }

    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}