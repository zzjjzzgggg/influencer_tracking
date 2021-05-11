//
// Created by weiwei on 2021/5/4.
//

#include "sieve_pag.h"
#include "dyn_bgraph_mgr.h"
#include "iset_generator.h"
#include <gflags/gflags.h>

int main() {
    int budget = 10;
    double eps = 0.1;
    int num_samples = 10;
    double p = 0.6;
    SievePAG<DynBGraphMgr> pag{num_samples, budget, eps};

    SocialAcs ss;
    std::string filename = "Gowalla_Checkins.txt";
    std::ifstream data(filename);
    std::string oneline;
    int x = 0;
    SocialAcs sc;
    while (getline(data, oneline)) {
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while (read_str >> item)
            temp.push_back(int(item));
        SocialAc soca = std::make_pair(std::make_pair(temp[0], temp[1]), temp[2]);
        sc.push_back(soca);
        x++;
        if(x==100)
            break;
    }

    for(auto &s:sc){
        ISetGenerator isgen(num_samples,p);
        ISet is=isgen.getISet();
        pag.update(s,is);

        double pag_mx=pag.getResult();
        std::cout<<pag_mx<<std::endl;
        pag.clear();
    }
}