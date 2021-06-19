//
// Created by weiwei on 2021/5/29.
//

#include "sieve_pait.h"
#include "iset_generator.h"
#include "stackexchange_obj_fun.h"

#include <gflags/gflags.h>

DEFINE_string(dir, "", "working directory");
DEFINE_string(stream, "comment_post.txt", "input streaming data file name");
DEFINE_string(obj, "output.txt", "objective file name");
DEFINE_int32(n, 10, "number of samples");
DEFINE_int32(B, 10, "budget");
DEFINE_double(eps, 0.2, "epsilon");
DEFINE_double(p, 0.6, "probability");

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    SievePAIT<StackExObjFun> sieve(FLAGS_n, FLAGS_B, FLAGS_eps);

    ISetGenerator isgen(FLAGS_n, FLAGS_p);

    std::vector<std::tuple<int, double>> rst;
    int temp = 1;
    ioutils::TSVParser ss(FLAGS_stream);
    while (ss.next()) {
        int u = ss.get<int>(0), c = ss.get<int>(1), t = ss.get<int>(2);
        Action a;  // TODO: create an social action using u,c,t,...

        ISet iset = isgen.getISet();
        sieve.update(a, iset);

        double val = sieve.getResult();
        int num_calls = sieve.getOracleCalls();

        std::cout << val << std::endl;
        //        std::cout<<num_calls<<std::endl;
        sieve.clear();
        rst.emplace_back(temp, val);
    }

    std::string ofnm = osutils::join(
        FLAGS_dir,
        "pait_n{}b{}eps{}p{}.dat"_format(FLAGS_n, FLAGS_B, FLAGS_eps, FLAGS_p));
    ioutils::saveTupleVec(rst, ofnm, "{}\t{}\n");

    gflags::ShutDownCommandLineFlags();
    return 0;
}
