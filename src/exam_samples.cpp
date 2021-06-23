//
// Created by weiwei on 2021/6/7.
//

#include "stackexchange_obj_fun.h"
#include "obj_mgr.h"
#include "iset_generator.h"

#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_int32(n, 2000, "number of max samples");
DEFINE_int32(i, 3, "experiment index");
DEFINE_double(lmd, .01, "decaying rate");
DEFINE_int32(k, 10, "number of picked users");
DEFINE_int32(end, 1000, "end time");

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

    ObjMgr<StackExObjFun> obj(FLAGS_n);
    ISetGenerator gen;

    int t = 0;
    std::unordered_set<int> user_set;

    std::vector<Action> social_actions;
    ioutils::TSVParser ss(FLAGS_stream);
    while (ss.next()) {
        int c = ss.get<int>(0), u = ss.get<int>(1), v = ss.get<int>(2);
        social_actions.emplace_back(u, v, c, t);
        if (++t == FLAGS_end) break;
    }

    for (auto& a : social_actions) {
        ISet iset = gen.getISet(FLAGS_n, getP(FLAGS_lmd, FLAGS_end, a.t));
        if (!iset.empty()) {
            obj.update(a, iset);
            user_set.insert(a.u);
            user_set.insert(a.v);
        }
    }

    std::vector<int> pop(user_set.begin(), user_set.end());
    std::vector<int> picked_users = rngutils::choose(pop, FLAGS_k, rng);

    // the approximate true value
    double fts_true = obj.getVal(picked_users);
    std::cout << fts_true << std::endl;

    int sample_size = 1000;
    int r = 500;
    std::vector<std::tuple<int, double>> rst;
    for (int n = 10; n <= sample_size; n += 10) {
        std::vector<double> fts_est_vec;
        for (int j = 1; j <= r; j++) {
            ObjMgr<StackExObjFun> objmgr(n);

            for (auto& a : social_actions) {
                ISet iset = gen.getISet(n, getP(FLAGS_lmd, FLAGS_end, a.t));
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
        osutils::join(FLAGS_dir, "samples_n{}lmd{}k{}i{}.dat"_format(
                                     FLAGS_n, FLAGS_lmd, FLAGS_k, FLAGS_i));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");

    printf("cost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
