//
// Created by weiwei on 2021/6/6.
//

#ifndef INFLUENCERS_TRACKING_HISTIT_RED_H
#define INFLUENCERS_TRACKING_HISTIT_RED_H

#include "iset_segment.h"
#include "sieve_pait.h"

/**
 * HistITRED
 */
template <typename Fun>
class HistITRED {
private:
    class Alg {
    private:
        SievePAIT<Fun>* sieve_ptr_;

    public:
        int l_;                   // instance index
        double val_ = 0;          // current utility value
        double delta_ = 0;        // uncertainty of this instance
        double uncertainty_ = 0;  // uncertainty of this interval

    public:
        Alg(const int l, const int num_samples, const int budget,
            const double eps)
            : l_(l) {
            sieve_ptr_ = new SievePAIT<Fun>(num_samples, budget, eps);
        }
        Alg(const Alg& o) : l_(o.l_), val_(o.val_) {
            sieve_ptr_ = new SievePAIT<Fun>(*o.sieve_ptr_);
        }
        virtual ~Alg() { delete sieve_ptr_; }
        inline int getOracleCalls() {
            return sieve_ptr_->getOracleCalls();
        }

        inline void feed(const Action& a, const ISet& is) {
            sieve_ptr_->update(a, is);
            val_ = sieve_ptr_->getResult();
        }

        inline double upper() const { return val_ + delta_; }
        inline double lower() const { return val_; }

        inline void clear() { sieve_ptr_->clear(); }
    };

private:
    int num_samples_, budget_;
    double red_eps_, sieve_eps_;

    std::list<Alg*> algs_;

public:
    HistITRED(const int num_samples, const int budget, const double red_eps,
              const double sieve_eps)
        : num_samples_(num_samples), budget_(budget), red_eps_(red_eps),
          sieve_eps_(sieve_eps) {}

    virtual ~HistITRED() {
        for (auto it = algs_.begin(); it != algs_.end(); ++it) delete *it;
    }

    void newEndIfNeed(const int l);
    int statOracleCalls();
    void feed(const Action& a, const ISetSegments& segs);

    void feedSegment(const Action& a, const ISetSegment& seg,
                     typename std::list<Alg*>::iterator& it);

    double getResult() const { return algs_.front()->val_; }
    void next();
    void reduce();
    int size()const {return algs_.size();}
};

template <typename Fun>
void HistITRED<Fun>::newEndIfNeed(const int l) {
    if (algs_.empty() || algs_.back()->l_ < l)
        algs_.push_back(new Alg(l, num_samples_, budget_, sieve_eps_));
}

template <typename Fun>
void HistITRED<Fun>::feed(const Action& a, const ISetSegments& segs) {
    // create a new head instance if necessary
    newEndIfNeed(segs.getMxIdx());
    auto it = algs_.begin();
    for (auto& seg : segs.segments_) {
//        std::cout<<seg.start_<<" "<<seg.end_<<std::endl;
        // Update instances belonging to this segment.
        feedSegment(a, seg, it);
        auto pre = it;
        // if last updated alg.l_=l,continue
        if (it != algs_.begin()) {
            --pre;  // let "pre" be the precessor of "it"
            // no need to create a new instance
            if ((*pre)->l_ == seg.end_ - 1) continue;
        }

        // create new alg from successor
        if(it==algs_.begin()||(*it)->lower()<(1-red_eps_)*(*pre)->upper()){
       // if (it == algs_.begin() || (*pre)->l_ < (seg.end_ - 1)) {
            // create alg_l based on its successor
            Alg* alg = new Alg(*(*it));
            alg->l_ = seg.end_ - 1;
            alg->delta_ = alg->uncertainty_ = (*it)->uncertainty_;
            alg->clear();
            alg->feed(a, seg.is_);
            algs_.insert(it, alg);
        }
    }
}

// Update instances belonging to this segment.
template <typename Fun>
void HistITRED<Fun>::feedSegment(const Action& a, const ISetSegment& seg,
                                 typename std::list<Alg*>::iterator& it) {
//    while (it != algs_.end() && (*it)->l_ < seg.end_) {
//        (*it)->feed(a, seg.is_);
//        ++it;
//    }
    auto job = [a, &seg](Alg* alg) { alg->feed(a, seg.is_); };
    std::vector<std::future<void>> futures;
    while (it != algs_.end() && (*it)->l_ < seg.end_) {
        futures.push_back(std::async(std::launch::async, job, *it));
        ++it;
    }
    for (auto& future : futures) future.get();

}
template <typename Fun>
int HistITRED<Fun>::statOracleCalls() {
    int oracle_calls = 0;
    for (auto it = algs_.begin(); it != algs_.end(); ++it)
        oracle_calls += (*it)->getOracleCalls();
    return oracle_calls;
}
template <typename Fun>
void HistITRED<Fun>::next() {
    // If head SievePAIT instance expires
    if (algs_.front()->l_ == 0) {
        delete algs_.front();
        algs_.pop_front();
    }
    // clear remaining SievePAIT instances
    for (auto it = algs_.begin(); it != algs_.end(); ++it) {
        (*it)->clear();
        (*it)->l_--;
    }
}

template <typename Fun>
void HistITRED<Fun>::reduce() {
    auto i = algs_.begin();
    // for each i,find the largest j such that A(j) >= (1-eps)(A(i)+beta(j)).
    while (i != algs_.end()) {
        auto j = i, l = i;
        if (++j == algs_.end()) break;
        // calculate the bound
        double bound = (*i)->upper() * (1 - red_eps_);
        while (j != algs_.end() && (*j)->lower() >= bound) ++j;
        if (--j == i || ++l == j) {
            ++i;
            continue;
        }
        // Now (*l) points to the first instance to be deleted, (*j) points to
        // the last instance that should be kept.

        // (*i)->uncertainty_ = (*l)->uncertainty_;
        while (l != j) {
            delete *l;
            ++l;
        }
        // get the uncertainty of this interval
        (*j)->uncertainty_ = (*i)->upper() * red_eps_;
        algs_.erase(++i, j);
        i = j;
    }
}

#endif  // INFLUENCERS_TRACKING_HISTIT_RED_H
