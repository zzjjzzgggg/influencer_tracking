//
// Created by weiwei on 2021/6/7.
//

#include "checkin_obj_fun.h"
//#include "stackexchange_obj_fun.h"
#include "obj_mgr.h"
#include "iset_generator.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "../../data/sample/stackoverflow", "working directory");
DEFINE_string(stream, "stackoverflow.txt", "input streaming data file name");
DEFINE_int32(n, 2000, "number of max samples");
DEFINE_int32(i, 1, "experiment index");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(k, 50, "number of picked users");
DEFINE_int32(T, 1000, "end time");
DEFINE_string(user,"stoverflow_i1.txt","random picked users");

inline double getP(double lambda, int time, int ta) {
    return exp(-lambda * (time - ta));
}

double getMSE(std::vector<double> fts_est_vec, double fts_true) {
    double mse_sum = 0;
    for (auto est : fts_est_vec) mse_sum += pow(est - fts_true, 2);
    return mse_sum / fts_est_vec.size();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    rngutils::default_rng rng;

    ObjMgr<CheckinObjFun> obj(FLAGS_n);
//    ObjMgr<StackExObjFun> obj(FLAGS_n);
    ISetGenerator gen;

    int t = 0;
    std::unordered_set<int> user_set;

    std::vector<Action> social_actions;
    ioutils::TSVParser ss(FLAGS_stream);
    while (ss.next()) {
        int c = ss.get<int>(0), u = ss.get<int>(1), v = ss.get<int>(2);
        Action a{u, v, c, t};
        social_actions.emplace_back(a);
        if (++t == FLAGS_T) break;
    }

    for (auto& a : social_actions) {
        ISet iset = gen.getISet(FLAGS_n, getP(FLAGS_lmd, FLAGS_T, a.t));
        if (!iset.empty()) {
            obj.update(a, iset);
            user_set.insert(a.u);
//            user_set.insert(a.v);
        }
    }


     /**
      * get the user for sample experiment
      */
//    std::vector<int> pop(user_set.begin(), user_set.end());
//    std::vector<int> picked_users = rngutils::choose(pop, FLAGS_k, rng);

    /**
     * get the user for lambda user experiment
     */
    ioutils::TSVParser uu(FLAGS_user);
    std::vector<int> picked_users;
    while(uu.next()){
        int user=uu.get<int>(0);
        picked_users.push_back(user);
    }

    // the approximate true value
    double fts_true = obj.getVal(picked_users);

    int sample_size = 1000;
    int r = 500;
    std::vector<std::tuple<int, double>> rst;
    for (int n = 10; n <= sample_size; n += 10) {
        std::vector<double> fts_est_vec;
        for (int j = 1; j <= r; j++) {
//            ObjMgr<StackExObjFun> objmgr(n);
            ObjMgr<CheckinObjFun> objmgr(n);

            for (auto& a : social_actions) {
                ISet iset = gen.getISet(n, getP(FLAGS_lmd, FLAGS_T, a.t));
                objmgr.update(a, iset);
            }

            double val = objmgr.getVal(picked_users);
            fts_est_vec.push_back(val);
        }
        double mse = getMSE(fts_est_vec, fts_true);
        double nrmse = sqrt(mse) / fts_true;
        rst.emplace_back(n, nrmse);
    }

    std::string ofnm =
        osutils::join(FLAGS_dir, "samples_n{}lmd{}k{}i{}T{}fts_true{}.dat"_format(
                                     FLAGS_n, FLAGS_lmd, FLAGS_k, FLAGS_i,
                                    strutils::prettyNumber(FLAGS_T),fts_true));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");

    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
