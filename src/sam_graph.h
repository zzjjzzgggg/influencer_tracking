//
// Created by weiwei on 2021/4/23.
//

#ifndef TEST_CMAKE_SAM_GRAPH_H
#define TEST_CMAKE_SAM_GRAPH_H

class SamGraph:public HyperANF{
private:
    dir::DGraph graph_;
    std::vector<int> nodes;
public:
    SamGraph(){}
    inline bool exist(const int n){
        bool flag=std::find(nodes.begin(),nodes.end(),n)!=nodes.end();
        return flag;
    }
    void addEdge(const int u, const int v)  ;
    double getReward(const int node)  {
        if(exist(node))
        return estimate(node);
        else
            return 0;
    }
};
void SamGraph::addEdge(const int u, const int v) {
    graph_.addEdge(u, v);
    if(!exist(u)){
        nodes.push_back(u);
    }
    if(!exist(v)){
        nodes.push_back(v);
    }
}


#endif //TEST_CMAKE_SAM_GRAPH_H
