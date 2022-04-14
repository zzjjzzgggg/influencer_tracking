//
// Created by zhangwei on 2021/4/20.
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
#include <future>

#include <os/osutils.h>
#include <io/ioutils.h>
#include <adv/rngutils.h>
#include <graph/graph.h>
#include <adv/LRUCache.h>

using namespace fmt::literals;
using namespace graph;

using ISet = std::vector<int>;

// represents a social action
struct Action {
    int u;
    int v;
    int t;
};

#endif  // INFLUENCERS_TRACKING_STDAFX_H
