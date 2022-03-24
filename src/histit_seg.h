//
// Created by zhangwei on 2021/6/3.
//

#ifndef INFLUENCERS_TRACKING_HISTIT_SEG_H
#define INFLUENCERS_TRACKING_HISTIT_SEG_H

#include "iset_segment.h"
#include "sieve_pait.h"

/**
 * HistITSEG
 */
template <typename Fun,typename InputData>
class HistITSEG{
private:
    class Alg{
    private:
        SievePAIT<Fun,InputData>* sieve_ptr_;
    public:
        int l_;//instance index
        double val_=0;//current utility value

    public:
        Alg(const int l,const int num_samples,const int budget,const double eps):l_(l){
            sieve_ptr_=new SievePAIT<Fun,InputData>(num_samples,budget,eps);
        }
        Alg(const Alg& o) : l_(o.l_), val_(o.val_) {
            sieve_ptr_ = new SievePAIT<Fun,InputData>(*o.sieve_ptr_);
        }

        virtual ~Alg(){delete sieve_ptr_;}
        inline int getOracleCalls() { return sieve_ptr_->getOracleCalls(); }

        inline void feed(const InputData &a, const ISet& is) {
            sieve_ptr_->update(a, is);
            val_ = sieve_ptr_->getResult();
        }
        inline void clear(){sieve_ptr_->clear();}
    };

private:
    int num_samples_;
    int budget_;
    double eps_;

    std::list<Alg*> algs_;
public:
    HistITSEG(const int num_samples, const int budget, const double eps)
            : num_samples_(num_samples), budget_(budget), eps_(eps) {
    }

    virtual ~ HistITSEG() {
        for (auto it = algs_.begin(); it != algs_.end(); ++it) delete *it;
    }

    void newEndIfNeed(const int l);
    int statOracleCalls();
    void feed(const InputData &a,const ISetSegments& segs);

    void feedSegment(const InputData &a, const ISetSegment& seg,
                     typename std::list<Alg*>::iterator& it);

    double getResult() const { return algs_.front()->val_; }
    void next();
};
/**
   * Create a new instance at the tail if need.
   * - "idx": the maximum position that an instance should exist.
 */
template <typename Fun,typename InputData>
void HistITSEG<Fun,InputData>::newEndIfNeed(const int l) {
    if (algs_.empty() || algs_.back()->l_ < l)
        algs_.push_back(new Alg(l, num_samples_, budget_, eps_));
}

template <typename Fun,typename InputData>
void HistITSEG<Fun,InputData>::feed(const InputData &a, const ISetSegments& segs){
    //create a new head instance if necessary
    newEndIfNeed(segs.getMxIdx());
    auto it=algs_.begin();
    for(auto& seg:segs.segments_){
        //Update instances belonging to this segment.
        feedSegment(a,seg,it);
        auto pre=it;
        //if last updated alg.l_=l,continue
        if (it != algs_.begin()) {
            --pre;  // let "pre" be the precessor of "it"
            // no need to create a new instance
            if ((*pre)->l_ == seg.end_ - 1) continue;
        }

        //create new alg from successor
        if(it==algs_.begin()||(*pre)->l_<(seg.end_-1)){
            //create alg_l based on its successor
            Alg* alg=new Alg(*(*it));
            alg->l_=seg.end_-1;
            alg->clear();
            alg->feed(a,seg.is_);
            algs_.insert(it,alg);
        }
    }
}

// Update instances belonging to this segment.
template <typename Fun,typename InputData>
void HistITSEG<Fun,InputData>::feedSegment(const InputData &a, const ISetSegment& seg,
                                 typename std::list<Alg*>::iterator& it) {
    auto job = [a, &seg](Alg* alg) { alg->feed(a, seg.is_); };
    std::vector<std::future<void>> futures;
    while (it != algs_.end() && (*it)->l_ < seg.end_) {
        futures.push_back(std::async(std::launch::async, job, *it));
        ++it;
    }
    for (auto& future : futures) future.get();
}
template <typename Fun,typename InputData>
int HistITSEG<Fun,InputData>::statOracleCalls() {
    int oracle_calls = 0;
    for (auto it = algs_.begin(); it != algs_.end(); ++it)
        oracle_calls += (*it)->getOracleCalls();
    return oracle_calls;
}

template <typename Fun,typename InputData>
void HistITSEG<Fun,InputData>::next() {
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


#endif //INFLUENCERS_TRACKING_HISTIT_SEG_H
