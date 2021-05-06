//
// Created by weiwei on 2021/4/29.
//

#include "greedy_alg.h"
#include "bernoulli_get.h"


int main(){
    int budget=10;
    double eps=0.2;
    int num_samples=10;
    double p=0.6;

    GreedyAlg greedy{num_samples,budget};

    std::string filename="output2.txt";
    std::ifstream data(filename);
    std::string oneline;
    int x=0;
    SocialAcs ss;
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
        if(x==200)
            break;
    }
    ss.push_back({{1,2},1619009223});

//    ss.push_back({{1,2},1619009211});
//    ss.push_back({{1,2},1619009211});
    ss.push_back({{5,2},1619009211});
//////
//    ss.push_back({{1,2},1619009224});
//
    ss.push_back({{4,5},1619009223});
//    ss.push_back({{4,5},1619009223});
//
//
    ss.push_back({{5,8},1619009223});
//    ss.push_back({{2,7},1619009223});

    ss.push_back({{2,7},1619009223});
    ss.push_back({{2,8},1619009223});
    ss.push_back({{1,3},1619009211});
    ss.push_back({{2,9},1619009211});
    for(auto &s:ss){
        BernoulliSetGenerator bsgen(num_samples,p);
        BernoulliSet bs=bsgen.getBernoulliSet();
        std::vector<int> nodes=greedy.update(s,bs);

//        std::vector<int> St=greedy.getResult(nodes);
//        for(auto &item:St){
//            std::cout<<item<<" ";
//        }
//        std::cout<<std::endl;
        double greedy_gain=greedy.getResult(nodes);
        std::cout<<"greedy:"<<greedy_gain<<std::endl;
    }

        return 0;
}

