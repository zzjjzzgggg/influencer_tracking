//
// Created by weiwei on 2021/4/18.
//

#ifndef SIEVEPAG_SOCIAL_ACTION_H
#define SIEVEPAG_SOCIAL_ACTION_H

#endif //SIEVEPAG_SOCIAL_ACTION_H

#include "stdafx.h"
#include "bernoulli_get.h"

class SocialAction{
private:
    Edge edge;
    int timestamp;
public:
    SocialAction(Edge &edge,int &timestamp);

};
SocialAction::SocialAction(Edge &edge, int &timestamp) {
    this->edge=edge;
    this->timestamp=timestamp;
}

