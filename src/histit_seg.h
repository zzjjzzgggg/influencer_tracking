//
// Created by weiwei on 2021/6/3.
//

#ifndef INFLUENCERS_TRACKING_HISTIT_SEG_H
#define INFLUENCERS_TRACKING_HISTIT_SEG_H

#include "iset_segment.h"
#include "sieve_pait.h"

class HistITSEG{
private:
    class Alg{
    private:
        SievePAIT* sieve_ptr_;
    public:
        int l_;
        double val_=0;
    public:
        Alg(const int l,const int num_samples,const int budget,const double eps):l_(l){
            sieve_ptr_=new SievePAIT(num_samples,budget,eps);
        }
        Alg(const Alg& o) : l_(o.l_), val_(o.val_) {
            sieve_ptr_ = new SievePAIT(*o.sieve_ptr_);
        }

        virtual ~Alg(){delete sieve_ptr_;}

        inline void feed(const UVC &a, const ISet& is) {
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

    void feed(const UVC &a,const ISetSegments& segs);

    void feedSegment(const UVC &a, const ISetSegment& seg,
                     typename std::list<Alg*>::iterator& it);

    double getResult() const { return algs_.front()->val_; }
    void next();
    void reduce();
    void process(const SocialAc a, const int l, const int r, const ISetSegment& seg);
};

void HistITSEG::newEndIfNeed(const int l) {
    if (algs_.empty() || algs_.back()->l_ < l)
        algs_.push_back(new Alg(l, num_samples_, budget_, eps_));
}

void HistITSEG::feed(const UVC &a,const ISetSegments& segs){
    //create a new tail instance if necessary
    newEndIfNeed(segs.getMnIdx());

    auto it=algs_.begin();
    for(auto& seg:segs.segments_){
        //Update instances belonging to this segment.
        feedSegment(a,seg,it);

        if(it==algs_.end())return;
        auto pre=it;
        //if last updated alg.l_=l,continue
        if (it != algs_.begin()) {
            --pre;  // let "pre" be the precessor of "it"
            // no need to create a new instance
            if ((*pre)->l_ == seg.end_ - 1) continue;
        }

        //create new alg before succ
        if(it==algs_.begin()||(*pre)->l_<(seg.end_-1)){
         // if(it==algs_.begin()||(*it)->val_<(1-eps_)*(*pre)->val_){
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
void HistITSEG::feedSegment(const UVC &a, const ISetSegment& seg,
                            std::list<Alg*>::iterator& it) {
//    auto job = [a, &seg](Alg* alg) { alg->feed(a, seg.is_ ); };
//    std::vector<std::future<void>> futures;
    while (it != algs_.end() && (*it)->l_ < seg.end_) {
//        futures.push_back(std::async(std::launch::async, job, *it));
        (*it)->feed(a, seg.is_);

        ++it;
    }
//    for (auto& future : futures) future.get();
}

void HistITSEG::next() {
    // If head SievePAG instance expires
    if (algs_.front()->l_ == 0) {
        delete algs_.front();
        algs_.pop_front();
    }
    // clear remaining SievePAG instances
    for (auto it = algs_.begin(); it != algs_.end(); ++it) {
        (*it)->clear();
        (*it)->l_--;
    }
}

void HistITSEG::reduce() {
    // For each i, find the largest j such that A(j) >= (1-e)A(i)
    auto start = algs_.begin();
    while (start != algs_.end()) {
        auto last = start, it = start;
        double bound = (*start)->val_ * (1 - eps_);
        while(true){
            ++it;
            if(it==algs_.end()||(*it)->val_<bound)break;
            ++last;
        }
        if(start!=last){
            it=start;
            while(++it!=last){
                delete *it;
            }
            algs_.erase(++start,last);
            start = last;
        }else
            ++start;
    }
}


#endif //INFLUENCERS_TRACKING_HISTIT_SEG_H
