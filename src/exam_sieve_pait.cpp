//
// Created by weiwei on 2021/5/29.
//

//#include "subgraph.h"
#include "sieve_pait.h"
#include "iset_generator.h"

int main(){
    int budget=10;
    double eps=0.2;
    int num_samples=10;
    double p=0.3;

    UVCs social_actions;

    //postid postuserid commentuserid timestamp
    std::string filename = "comment_post.txt";

    std::ifstream data(filename);
    std::string oneline;
    int x = 0;
    while (getline(data, oneline)) {
        std::istringstream read_str(oneline);
        int item;
        std::vector<int> temp;
        while (read_str >> item)
            temp.push_back(int(item));
        UVC soca = std::make_pair(std::make_pair(temp[1], temp[2]), temp[0]);
        social_actions.push_back(soca);
        x++;
        if(x==100)
            break;
    }

    SievePAIT pait{num_samples,budget,eps};
    for(auto &s:social_actions){
        ISetGenerator isgen(num_samples,p);
        ISet is=isgen.getISet();
        pait.update(s,is);

        double pag_mx=pait.getResult();
        std::cout<<pag_mx<<std::endl;
        pait.clear();
    }

    return 0;
}