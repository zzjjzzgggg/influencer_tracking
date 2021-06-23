//
// Created by weiwei on 2021/6/7.
//

#include "stackexchange_obj_fun.h"
#include <gflags/gflags.h>
#include "samples.h"
#include "obj_mgr.h"

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_int32(n, 2000, "number of max samples");
DEFINE_int32(i,1,"experiment index");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(k,10,"number of picked users");
DEFINE_int32(end,1000,"end time");


int main(int argc, char* argv[]){
    rngutils::default_rng rng;
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    ObjMgr<StackExObjFun> obj(FLAGS_n);
    ioutils::TSVParser ss(FLAGS_stream);
    int t=0;
    std::vector<int> users;
    while (ss.next()){
        ++t;
        int c = ss.get<int>(0), u = ss.get<int>(1), v=ss.get<int>(2), ta= ss.get<int>(3);
        Action a{u,v,c,ta};
        std::vector<int> iset;
        for(int k=0;k<FLAGS_n;k++){
            if(rng.uniform() <= get_prob(FLAGS_lmd,FLAGS_end,ta)){
                iset.push_back(k);
            }
        }
        obj.update(a,iset);

        if(std::find(users.begin(),users.end(),u)==users.end()){
            users.push_back(u);
        }
        if(std::find(users.begin(),users.end(),v)==users.end()){
            users.push_back(v);
        }
        if(t==FLAGS_end) break;
    }

    std::vector<int> picked_users;
    picked_users=rngutils::choose(users, FLAGS_k, rng);

    //the approximate true value
    double fts_true=obj.getVal(picked_users);
    std::cout<<fts_true<<std::endl;

    int sample_size=1000;
    int r=500;
    std::vector<std::tuple<int,double>> rst;
    for(int i=10;i<=sample_size;i+=10){
        std::cout<<i<<std::endl;
        std::vector<double> fts_set;
        for(int j=1;j<=r;j++){
            int temp=0;
            ObjMgr<StackExObjFun> objmgr(i);
            ioutils::TSVParser ss1(FLAGS_stream);
            while (ss1.next()&&(++temp)<=1000){
                int c = ss1.get<int>(0), u = ss1.get<int>(1), v=ss1.get<int>(2), ta= ss1.get<int>(3);
                Action a{u,v,c,ta};
                std::vector<int> iset;
                for(int k=0;k<i;k++){
                    if(rng.uniform() <= get_prob(FLAGS_lmd,FLAGS_end,ta)){
                        iset.push_back(k);
                    }
                }
                objmgr.update(a,iset);
            }
            double val=objmgr.getVal(picked_users);
            fts_set.push_back(val);
        }
        double mse= get_mse(fts_set,fts_true);
        double nrmse= sqrt(mse)/fts_true;
        rst.emplace_back(i,nrmse);
    }

    std::string ofnm = osutils::join(
            FLAGS_dir,
            "samples_n{}lmd{}k{}i{}fts_true{}.dat"_format(FLAGS_n, FLAGS_lmd,FLAGS_k,FLAGS_i,fts_true
            ));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");
    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}



