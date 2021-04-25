//
// Created by weiwei on 2021/4/20.
//

#ifndef TEST_CMAKE_STDAFX_H
#define TEST_CMAKE_STDAFX_H

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cmath>

#include <limits>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <experimental/tuple>
#include <stdlib.h>

#include <mutex>
#include <future>
#include <thread>

#include <adv/LRUCache.h>
#include <os/osutils.h>
#include <io/ioutils.h>
#include <adv/rngutils.h>
#include <graph/graph.h>

using namespace fmt::literals;


using ISet = std::vector<int>;

using namespace graph;

typedef std::pair<int,int> Edge;
typedef std::pair<Edge,int> SocialAc;
typedef std::vector<SocialAc> SocialAcs;
//typedef std::vector<std::pair<int,int>> SocialAction;
using BernoulliSet = std::vector<int>;


typedef std::pair<int, int> IntPr;
typedef std::vector<std::pair<int, int>> IntPrV;

#endif //TEST_CMAKE_STDAFX_H
