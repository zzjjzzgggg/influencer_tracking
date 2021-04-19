//
// Created by weiwei on 2021/4/18.
//

#ifndef SIEVEPAG_SOCIAL_STREAM_H
#define SIEVEPAG_SOCIAL_STREAM_H

#endif //SIEVEPAG_SOCIAL_STREAM_H

#include "stdafx.h"
#include "social_action.h"

class SocialStream{
public:
    int num;
    //store the social action
    std::vector<SocialAction> social_actions;
public:
    SocialStream(std::vector<SocialAction> &S);
};
SocialStream::SocialStream(std::vector<SocialAction> &S) {
    int num=0;
    for(auto &item:S){
        this->social_actions.push_back(item);
        num++;
    }
    this->num=num;
}

//social stream,budget k,theta,
//定义socialstream的类，包含edge_num和vector的socialaction
//socialaction类包含edge和timestamp
//对每个social stream中的每个action e得到一个时刻t的I-set集合
//得到graph结构计算Ft
//get Ve集合
//计算出最大的delta
//得到新的theta集合之后，update S theta
//对Ve中的每个节点,以及theta集合中的每个阈值
//如果candidate的数量是小于k个，并且这个节点的增益值大于theta的
//这个节点v加入这个theta的集合
//使用monte-carlo 得到Ft的最大值