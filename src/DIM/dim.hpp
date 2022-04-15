#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <tuple>

#define ERROR "\x1b[41mERROR: "
#define WARN "\x1b[45mWARN: "
#define INFO "\x1b[32m\x1b[1mINFO: "
#define DEF "\x1b[0m\n"

using namespace std;
typedef long long LL;
typedef unsigned long long ULL;

using Edge = std::pair<int, int>;

class hedge {
public:
    set<int> H;
    vector<pair<int, int>> par;
    vector<pair<int, int>> x;  // <to, from>
    int z;
};

// Dynamic Influence Maximization
class DIM {
private:
    set<int> V;
    vector<int> Vvec;

    map<pair<int, int>, double> ps;
    // u->v, p
    map<pair<int, int>, double> rs;
    // v<-u, p

    int beta;
    ULL TOT;

    // DO NOT call if |V|=0
    void _adjust();

    bool _erase_node(ULL at, int v);
    bool _erase_edge(ULL at, int u, int v);
    void _shrink(ULL at);

    void _expand(ULL at, int z);

    void _change(int u, int v, double p);

    int _next_target(ULL at);

    void _clear(ULL at);
    void _weight(ULL at, int dw);
    double _get_R();

    class Xorshift {
    public:
        Xorshift();
        Xorshift(int seed);
        int _(int s, int i);
        // 32bit signed
        inline int gen_int();
        // error = O(n*2^-32)
        inline int gen_int(int n);
        // [0, 1) (53bit)
        inline double gen_double();

    private:
        unsigned int x, y, z, w;
    };

    Xorshift rng;

public:
    vector<hedge> hs;
    vector<int> ws;
    void init();

    map<int, int> ideg;

    map<int, set<ULL>> index;
    // v -> indices of h

    bool naive_operation;  // Naive vert add, vert del, & edge del

    void set_beta(int beta);

    inline bool exist_node(int u) { return V.count(u); }
    inline bool exist_edge(int u, int v) {
        return ps.count(std::make_pair(u, v));
    }

    std::tuple<std::vector<Edge>, std::vector<Edge>, std::vector<Edge>> cmp_edges(
        const std::map<Edge, double> &new_edges);

    // Dynamic updates
    void insert(int v);  // Insert Node v
    void erase(int v);   // Delete Node v (and its incident edges)
    void insert(int u, int v, double p);  // Insert Edge (u,v) with prob p
    void erase(int u, int v);             // Delete Edge (u,v)
    void change(int u, int v, double p);  // Change prob of Edge (u,v) to 0.9

    // Queries
    vector<int> infmax(int k);  // Extract an influential set of k vertices
    double infest_naive(vector<int> &S);  // Estimate the influence of S (slow.)
    double infest(vector<int> &S);        // Estimate the influence of S (fast.)
    double infest(int v);                 // Estimate the influence of v
};
