//
// Created by weiwei on 2021/4/21.
//

#include "sieve_pag.h"
//#include "dyn_dgraph_mgr.h"
#include "bernoulli_get.h"
#include <gflags/gflags.h>
#include "greedy_alg.h"


int main(){
    int budget=10;
    double eps=0.1;
    int num_samples=10;
    double p=0.6;
    SievePAG pag{num_samples,budget,eps};
    GreedyAlg greedy{num_samples,budget};

    SocialAcs ss;
//    std::ifstream data;
//    data.open("test.txt",std::ios::in);
//    std::string filename="Gowalla_totalCheckins.txt";
    std::string filename="sx-stackoverflow-c2a.txt";
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

//        std::cout<<oneline<<std::endl;
//        getline(read_str,item,'\t');
        SocialAc soca=std::make_pair(std::make_pair(temp[0],temp[1]),temp[2]);
//        std::cout<<item[0]<<" ";
//        std::cout<<item[1]<<" ";
//        std::cout<<item[2]<<" ";
        sc.push_back(soca);
        x++;
        if(x==1000)
            break;
    }
    ss.push_back({{1,2},1619009223});

//    ss.push_back({{1,2},1619009211});
//    ss.push_back({{1,2},1619009211});
    ss.push_back({{5,2},1619009211});
//////
    ss.push_back({{1,2},1619009224});

    ss.push_back({{4,5},1619009223});
    ss.push_back({{4,5},1619009223});


    ss.push_back({{5,8},1619009223});
    ss.push_back({{2,7},1619009223});

    ss.push_back({{2,7},1619009223});
    ss.push_back({{2,8},1619009223});
    ss.push_back({{1,3},1619009211});
    ss.push_back({{2,9},1619009211});
    ss.push_back({{2,3},1619009211});
    ss.push_back({{2,3},1619009211});
    ss.push_back({{0,3},1619009211});
    ss.push_back({{0,3},1619009211});
    ss.push_back({{10,0},1619009211});
    ss.push_back({{8,11},1619009211});
    ss.push_back({{11,18},1619009211});

    int temp=1;
    std::string outfile="data1.txt";
    std::ofstream out(outfile);
    for(auto &s:sc){
        BernoulliSetGenerator bsgen(num_samples,p);
        BernoulliSet bs=bsgen.getBernoulliSet();

//        pag.feed(s,bs);
        pag.update(s,bs);
//        std::vector<int> St=pag.getResult();
//        std::cout<<temp<<" ";
        double pag_mx=pag.getResult();
        std::cout<<"sieve_pag"<<pag_mx<<"  ";
//        std::cout<<"sieve_pag"<<std::endl;
//       for(auto &item:St){
//            std::cout<<item<<" ";
//        }
//        std::cout<<std::endl;

        std::vector<int> nodes=greedy.update(s,bs);

//        std::vector<int> x=greedy.getResult(nodes);
        double greedy_gain=greedy.getResult(nodes);
        std::cout<<"greedy:"<<greedy_gain<<std::endl;
//        std::cout<<"greedy_pag"<<std::endl;
//        for(auto &item:x){
//            std::cout<<item<<" ";
//        }
//        std::cout<<std::endl;
        out<<temp<<'\t'<<pag_mx<<'\t'<<greedy_gain<<std::endl;
        pag.clear();
        temp++;
    }

    return 0;
}