//
// Created by weiwei on 2021/4/20.
//

#ifndef INFLUENCERS_TRACKING_STDAFX_H
#define INFLUENCERS_TRACKING_STDAFX_H

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

#include <fstream>
#include <sstream>
#include <string>

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

typedef std::vector<std::pair<int, int>> IntPrV;

typedef std::pair<int, int> UV;
typedef std::tuple<UV, int, int> SocialAc;
typedef std::vector<SocialAc> SocialAcs;

// struct SocialAc{
//    int u;
//    int v;
//    int c;
//    int timestamp;
//};

// std::vector<s_c> S

#endif  // INFLUENCERS_TRACKING_STDAFX_H
