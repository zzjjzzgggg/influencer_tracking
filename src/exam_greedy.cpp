
//
// Created by weiwei on 2021/4/29.
//

#include "greedy_pait.h"
#include "iset_generator.h"

int main(){
    int budget=10;
    int num_samples=10;
    double p=0.6;

    GreedyAlg greedy{num_samples,budget};

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
        if(x==100)
            break;
    }

    for(auto &a:social_actions){
        ISetGenerator isgen(num_samples,p);
        ISet is=isgen.getISet();

        greedy.update(a,is);

        double greedy_gain=greedy.getResult();
        std::cout<<"greedy:"<<greedy_gain<<std::endl;
        greedy.clear();
    }
    return 0;
}

