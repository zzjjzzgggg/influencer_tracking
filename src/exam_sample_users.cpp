//
// Created by weiwei on 2021/6/26.
//


#include "stackexchange_obj_fun.h"
#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "stackexchange.txt", "input streaming data file name");
DEFINE_int32(i, 10, "experiment index");
DEFINE_int32(k, 50, "number of picked users");
DEFINE_int32(end, 1000, "end time");

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;
    rngutils::default_rng rng;

    int t = 0;
    std::unordered_set<int> user_set;

    std::vector<Action> social_actions;
    ioutils::TSVParser ss(FLAGS_stream);
    while (ss.next()) {
        int c = ss.get<int>(0), u = ss.get<int>(1), v = ss.get<int>(2);
        Action a{u, v, c, t};
        social_actions.emplace_back(a);
        if (++t == FLAGS_end) break;
    }
    for(auto& a:social_actions){
        user_set.insert(a.u);
        user_set.insert(a.v);
    }
    std::vector<int> pop(user_set.begin(), user_set.end());
    std::vector<int> picked_users = rngutils::choose(pop, FLAGS_k, rng);
    std::unordered_set<int> users(picked_users.begin(),picked_users.end());
    std::string ofnm =
            osutils::join(FLAGS_dir, "stex_i{}.txt"_format(
                    FLAGS_i));
    ioutils::saveSet(users, ofnm, "{}\n");

    return 0;
}