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

// #include <mutex>
// #include <future>
// #include <thread>

#include <os/osutils.h>
#include <io/ioutils.h>
#include <adv/rngutils.h>
// #include <graph/graph.h>

using namespace fmt::literals;

using ISet = std::vector<int>;

typedef std::vector<std::pair<int, int>> IntPrV;

typedef std::pair<int, int> UV;
typedef std::tuple<UV, int, int> SocialAc;
typedef std::vector<SocialAc> SocialAcs;

// represents a social action
struct Action {
    int u;
    int v;
    int c;
    int t;
};

// std::vector<s_c> S

#endif  // INFLUENCERS_TRACKING_STDAFX_H
