//
// Created by weiwei on 2021/5/8.
//

#include "histit_seg.h"
#include "basic_it.h"
#include "lifespan_generator.h"
#include <gflags/gflags.h>
#include "stackexchange_obj_fun.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "stackexchange.txt", "input streaming data file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(L, 40, "maximum lifetime");
DEFINE_int32(end,1000,"end time");

int main(int argc, char* argv[]){
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    BasicIT<StackExObjFun> basic(FLAGS_L,FLAGS_B,FLAGS_eps,FLAGS_n);
    HistITSEG<StackExObjFun> hist(FLAGS_n,FLAGS_B,FLAGS_eps);
    LifespanGenerator lifegen(FLAGS_L,1-exp(-FLAGS_lmd));

    double sum=0;
    ioutils::TSVParser ss(FLAGS_stream);
    std::vector<std::tuple<int,double,double>> rst;
    int t=0;
    while(ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2), ta= ss.get<int>(3);
        Action a{u,v,c,ta};

        std::vector<int> lifespan=lifegen.getLifespans(FLAGS_n);
        ISetSegments segs(lifespan);
        hist.feed(a,segs);
        double hist_val=hist.getResult();
        std::cout<<t<<" ";
        std::cout << "hist" << hist_val << "   ";

        hist.next();

        basic.update(a,segs);
        double basic_val=basic.getResult();
        std::cout<<"basic"<<basic_val<<std::endl;
        basic.next();


        sum+= hist_val / basic_val;
        rst.emplace_back(t, basic_val, hist_val);
        if(t==FLAGS_end) break;
    }
    std::cout<<sum/t<<std::endl;
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "basic_and_hist_n{}b{}eps{}lmd{}L{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_eps,FLAGS_lmd,FLAGS_L));
    ioutils::saveTripletVec(rst, ofnm, "{}\t{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}