//
// Created by weiwei on 2021/5/30.
//

#include "sieve_pait.h"
#include "iset_generator.h"
#include <gflags/gflags.h>
#include "greedy_pait.h"


int main(){
    int budget=10;
    double eps=0.2;
    int num_samples=10;
    double p=0.6;
    SievePAIT pait{num_samples,budget,eps};
    GreedyAlg greedy{num_samples,budget};

    UVCs social_actions;

    std::string filename="comment_post.txt";
    std::ifstream data(filename);
    std::string oneline;
    int x=0;
    while(getline(data,oneline)){
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while(read_str>>item)
            temp.push_back(int(item));
        UVC soca=std::make_pair(std::make_pair(temp[1],temp[2]),temp[0]);
        social_actions.push_back(soca);
        x++;
        if(x==10000)
            break;
    }

    int temp=1;
    std::string outfile="data6_6greedy_pait.txt";
    std::ofstream out(outfile);
    double sum=0;
    for(auto &s:social_actions){
        ISetGenerator isgen(num_samples,p);
        ISet is=isgen.getISet();

        pait.update(s,is);
        double pait_mx=pait.getResult();
        std::cout<<temp<<" ";
        std::cout<<"sieve_pait"<<pait_mx<<"  ";

        pait.clear();

        greedy.update(s,is);
        double greedy_gain=greedy.getResult();

        std::cout<<"greedy:"<<greedy_gain<<std::endl;

        out<<temp<<'\t'<<pait_mx<<'\t'<<greedy_gain<<std::endl;
        greedy.clear();
        sum+=pait_mx/greedy_gain;
        temp++;
    }
    std::cout<<sum/x<<std::endl;
    return 0;
}

