#pragma once

#include <cstdio>
#include <cassert>
#include <cmath>

#include <limits>
#include <vector>
#include <unordered_map>

#include <mutex>
#include <future>
#include <thread>

#include <os/osutils.h>
#include <io/ioutils.h>
#include <adv/rngutils.h>
#include <graph/graph.h>

using namespace fmt::literals;
using ISet = std::vector<int>;

using namespace graph;

typedef std::pair<int,int> Edge;
//typedef std::vector<std::pair<int,int>> SocialAction;
using BernoulliSet = std::vector<int>;

//typedef std::pair<int, int> IntPr;
typedef std::vector<std::pair<int, int>> Edges;
