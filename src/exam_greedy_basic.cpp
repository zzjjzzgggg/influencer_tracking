//
// Created by weiwei on 2021/5/10.
//

#include "greedy_basic.h"
#include "lifespan_generator.h"

int main(){
    int L=40;
    int budget=10;
    int num_samples=10;

    double lmd=0.01;
    GreedyBasic greedy(L,budget,num_samples);

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
        if(x==300)
            break;
    }
    for(auto &a:social_actions){
        std::vector<int> lifespan=lifegen.getLifespans(num_samples);
        ISetSegments segs(lifespan);
        greedy.update(a,segs);

        double greedy_basic=greedy.getResult();
        std::cout<<greedy_basic<<std::endl;
        greedy.next();
    }

    return 0;
}