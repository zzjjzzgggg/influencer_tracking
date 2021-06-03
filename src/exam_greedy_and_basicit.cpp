//
// Created by weiwei on 2021/6/3.
//

#include "greedy_basic.h"
#include "lifespan_generator.h"
#include "basic_it.h"

int main(){
    int L=40;
    int budget=10;
    double eps=0.2;
    int num_samples=10;
    double p=0.6;

    double lmd=0.01;
    GreedyBasic greedy(L,budget,num_samples);
    BasicIT basic(L,budget,eps,num_samples);

    LifespanGenerator lifegen(L,1-exp(-lmd));

    std::string filename = "comment_post.txt";
    std::ifstream data(filename);
    std::string oneline;
    int x=0;
    UVCs social_actions;
    while(getline(data,oneline)){
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while(read_str>>item)
            temp.push_back(int(item));
        UVC soca=std::make_pair(std::make_pair(temp[1],temp[2]),temp[0]);
        social_actions.push_back(soca);
        x++;
        if(x==50000)
            break;
    }
    int temp=1;
    double ritio_sums=0;
//    std::string outfile="greedy_and_basic.txt";
//    std::ofstream out(outfile);
    for(auto &s:social_actions){
        std::vector<int> lifespan=lifegen.getLifespans(num_samples);
        ISetSegments segs(lifespan);
        greedy.update(s,segs);
//        std::cout<<temp<<" ";
        double greedy_basic=greedy.getResult();
        std::cout<<"greedy_basic "<<greedy_basic<<" ";
        greedy.next();

        basic.update(s,segs);

        double basic_it=basic.getResult();
        std::cout<<"basic_it "<<basic_it<<std::endl;
        basic.next();
//        out<<temp<<'\t'<<greedy_basic<<'\t'<<basic_pdg<<std::endl;
        ritio_sums+=basic_it/greedy_basic;
        temp++;
//        std::cout<<temp<<std::endl;
    }
    double ritio=ritio_sums/x;
    std::cout<<ritio;

    return 0;
}
