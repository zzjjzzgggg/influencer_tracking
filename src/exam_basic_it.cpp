//
// Created by weiwei on 2021/6/2.
//

#include "basic_it.h"
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
    BasicIT basic(FLAGS_L,FLAGS_B,FLAGS_eps,FLAGS_n);
    LifespanGenerator lifegen(FLAGS_L,1-exp(-FLAGS_lmd));

    SocialAcs social_actions;

    std::ifstream data(FLAGS_stream);
    std::string oneline;
    int x=0;
    while(getline(data,oneline)){
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while(read_str>>item)
            temp.push_back(int(item));
        social_actions.emplace_back(std::make_pair(temp[1],temp[2]),temp[0],temp[3]);
        x++;
        if(x==500)
            break;
    }
    int temp=1;
    std::vector<std::tuple<int,double>> rst;
    for(auto &a:social_actions){
        std::vector<int> lifespan=lifegen.getLifespans(FLAGS_n);
        ISetSegments segs(lifespan);
        basic.update(a,segs);

        double basic_mx=basic.getResult();
        std::cout<<basic_mx<<std::endl;
        basic.next();
        rst.emplace_back(temp,basic_mx);
        temp++;
    }
    std::string ofnm = osutils::join(
            FLAGS_dir,
            "basic_n{}b{}eps{}lmd{}L{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_eps,FLAGS_lmd,FLAGS_L));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}



