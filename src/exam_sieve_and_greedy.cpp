//
// Created by weiwei on 2021/4/21.
//

#include "sieve_pag.h"
#include "dyn_bgraph_mgr.h"
#include "iset_generator.h"
#include <gflags/gflags.h>
#include "greedy_alg.h"


int main(){
    int budget=10;
    double eps=0.2;
    int num_samples=10;
    double p=0.6;
    SievePAG<DynBGraphMgr> pag{num_samples,budget,eps};
    GreedyAlg<DynBGraphMgr> greedy{num_samples,budget};

    SocialAcs ss;

//    std::string filename="sx-stackoverflow-c2a.txt";
    std::string filename="Gowalla_Checkins.txt";
    std::ifstream data(filename);
    std::string oneline;
    int x=0;
    SocialAcs sc;
    while(getline(data,oneline)){
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while(read_str>>item)
            temp.push_back(int(item));
        SocialAc soca=std::make_pair(std::make_pair(temp[0],temp[1]),temp[2]);
        sc.push_back(soca);
        x++;
        if(x==1000)
            break;
    }

    int temp=1;
    std::string outfile="data1.txt";
    std::ofstream out(outfile);
    for(auto &s:sc){
        ISetGenerator isgen(num_samples,p);
        ISet is=isgen.getISet();

        pag.update(s,is);
        double pag_mx=pag.getResult();
        std::cout<<"sieve_pag"<<pag_mx<<"  ";

        pag.clear();

        greedy.update(s,is);
        double greedy_gain=greedy.getResult();

        std::cout<<"greedy:"<<greedy_gain<<std::endl;

        out<<temp<<'\t'<<pag_mx<<'\t'<<greedy_gain<<std::endl;
        greedy.clear();

        temp++;
    }

    return 0;
}