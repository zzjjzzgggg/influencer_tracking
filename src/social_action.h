//
// Created by weiwei on 2021/4/18.
//

#ifndef SIEVEPAG_SOCIAL_ACTION_H
#define SIEVEPAG_SOCIAL_ACTION_H



#include "stdafx.h"

class SocialAction{
public:
    SocialAc action;
    BernoulliSet bernoulliSet;
public:
    SocialAction(SocialAc action,BernoulliSet bernoulliSet);
};


#endif //SIEVEPAG_SOCIAL_ACTION_H
