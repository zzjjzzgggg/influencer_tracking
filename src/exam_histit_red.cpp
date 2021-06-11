//
// Created by weiwei on 2021/6/6.
//

#include "histit_seg.h"
#include "histit_red.h"
#include "iset_segment.h"
#include "lifespan_generator.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(L, 40, "maximum lifetime");

int main(){
    LifespanGenerator lifegen(FLAGS_L,1-exp(-FLAGS_lmd));
    HistITSEG hist(FLAGS_n,FLAGS_B,FLAGS_eps);
    HistITRED hist_red(FLAGS_n,FLAGS_B,FLAGS_eps);

    std::string oneline;
    int x=0;
    std::ifstream data(FLAGS_stream);
    SocialAcs social_actions;
    while(getline(data,oneline)){
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while(read_str>>item)
            temp.push_back(int(item));
        social_actions.emplace_back(std::make_pair(temp[1],temp[2]),temp[0],temp[3]);
        x++;
        if(x==5000)
            break;
    }

    int temp=1;

    std::vector<std::tuple<int,double,double>> rst;
    for(auto &a:social_actions){
        std::vector<int> lifespan=lifegen.getLifespans(FLAGS_n);
        ISetSegments segs(lifespan);

        hist.feed(a,segs);
        std::cout<<temp<<" ";
        double hist_mx=hist.getResult();
        std::cout<<"hist"<<hist_mx<<" ";
        hist.next();

        hist_red.feed(a, segs);
        double hist_red_mx=hist_red.getResult();
        std::cout<<"histred"<<hist_red_mx<<std::endl;

        hist_red.reduce();
        hist_red.next();
        rst.emplace_back(temp,hist_mx,hist_red_mx);
        temp++;
    }
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "hist_and_histred_n{}b{}eps{}lmd{}L{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_eps,FLAGS_lmd,FLAGS_L));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}
