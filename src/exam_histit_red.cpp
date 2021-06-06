//
// Created by weiwei on 2021/6/6.
//

#include "histit_seg.h"
#include "histit_red.h"
#include "iset_segment.h"
#include "lifespan_generator.h"

int main(){
    int L=40;
    int budget=10;
    double eps=0.2;
    int num_samples=10;

    double lmd=0.01;
    LifespanGenerator lifegen(L,1-exp(-lmd));
    std::string filename="comment_post.txt";
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
        if(x==5000)
            break;
    }

    HistITSEG hist(num_samples,budget,eps);
    HistITRED hist_red(num_samples, budget, eps);

    int i=1;

    std::string outfile="6_6hist_and_red.txt";
    std::ofstream out(outfile);
    for(auto &a:social_actions){
        std::vector<int> lifespan=lifegen.getLifespans(num_samples);
        ISetSegments segs(lifespan);

        hist.feed(a,segs);
        std::cout<<i++<<" ";
        double hist_mx=hist.getResult();
        std::cout<<"hist"<<hist_mx<<" ";
        hist.next();

        hist_red.feed(a, segs);
        double hist_red_mx=hist_red.getResult();
        std::cout<<"histred"<<hist_red_mx<<std::endl;

        hist_red.reduce();
        hist_red.next();
    }

    return 0;
}