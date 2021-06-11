//
// Created by weiwei on 2021/5/10.
//

#include "greedy_basic.h"
#include "lifespan_generator.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(L, 500, "maximum lifetime");

int main(){
    GreedyBasic greedy(FLAGS_L,FLAGS_B,FLAGS_n);
    LifespanGenerator lifegen(FLAGS_L,1-exp(-FLAGS_lmd));

    std::ifstream data(FLAGS_stream);
    std::string oneline;
    int x=0;

    SocialAcs social_actions;
    while(getline(data,oneline)){
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while(read_str>>item)
            temp.push_back(int(item));
        social_actions.emplace_back(std::make_pair(temp[1],temp[2]),temp[0],temp[3]);
        x++;
        if(x==300)
            break;
    }
    int temp=1;
    std::vector<std::tuple<int,double>> rst;
    std::ofstream out(FLAGS_obj);

    for(auto &a:social_actions){
        std::vector<int> lifespan=lifegen.getLifespans(FLAGS_n);
        ISetSegments segs(lifespan);
        greedy.update(a,segs);

        double greedy_basic=greedy.getResult();
        std::cout<<greedy_basic<<std::endl;
        rst.emplace_back(temp,greedy_basic);
        greedy.next();
        temp++;
    }

    std::string ofnm = osutils::join(
            FLAGS_dir,
            "greedy_basic_n{}b{}lmd{}L{}.dat"_format(FLAGS_n, FLAGS_B,FLAGS_L,FLAGS_L));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    gflags::ShutDownCommandLineFlags();
    return 0;
}