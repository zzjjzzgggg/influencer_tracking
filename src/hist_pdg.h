//
// Created by weiwei on 2021/5/6.
//

#ifndef INFLUENCERS_TRACKING_HIST_PDG_H
#define INFLUENCERS_TRACKING_HIST_PDG_H

#include "ISet_segment.h"
#include "sieve_pag.h"
#include "dyn_bgraph_mgr.h"
//template<class InputMgr>
class HistPDG{
private:
    class Alg{
    private:
        SievePAG<DynBGraphMgr>* sieve_ptr_;
    public:
        int l_;
        double val_;
    public:
        Alg(const int l,const int num_samples,const int budget,const double eps):l_(l){
            sieve_ptr_=new SievePAG<DynBGraphMgr>(num_samples,budget,eps);
        }

        virtual ~Alg(){delete sieve_ptr_;}

        inline void feed(const SocialAc s, const ISet& is) {
            sieve_ptr_->update(s, is);
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
    HistPDG(const int num_samples, const int budget, const double eps)
            : num_samples_(num_samples), budget_(budget), eps_(eps) {
    }

    virtual ~HistPDG() {
        for (auto it = algs_.begin(); it != algs_.end(); ++it) delete *it;
    }

    void newEndIfNeed(const int l);

    void feed(const SocialAc e,const ISetSegments& segs);

    void feedSegment(const SocialAc e, const ISetSegment& seg,
                     typename std::list<Alg*>::iterator& it);

    double getResult() const { return algs_.front()->val_; }
    void next();
};

/**
 *
 *create a new instance if need,l_<
 */
//template<class InputMgr>
void HistPDG::newEndIfNeed(const int l) {
    if (algs_.empty() || algs_.back()->l_ < l)
        algs_.push_back(new Alg(l, num_samples_, budget_, eps_));
}

//template<class InputMgr>
void HistPDG::feed(const SocialAc e,const ISetSegments& segs){
    //create a new tail instance if necessary
    newEndIfNeed(segs.getMnIdx());

    auto it=algs_.begin();
    for(auto& seg:segs.segments_){
        // Update instances belonging to this segment.
        feedSegment(e,seg,it);

        auto pre=it;
        //if last updated alg.l_=l,return
        if (it != algs_.begin()) {
            --pre;  // let "pre" be the precessor of "it"
            // no need to create a new instance
            if ((*pre)->l_ == seg.end_ - 1) continue;
        }

        //create new alg before succ
        if(it==algs_.begin()){
            //create alg_l based on its successor
            Alg* alg=new Alg(*(*it));
            alg->l_=seg.end_-1;
            alg->clear();
            alg->feed(e,seg.is_);
            algs_.insert(it,alg);
        }
    }

}
//template<class InputMgr>
// Update instances belonging to this segment.
void HistPDG::feedSegment(const SocialAc e, const ISetSegment& seg,
                              std::list<Alg*>::iterator& it) {
    auto job = [e, &seg](Alg* alg) { alg->feed(e, seg.is_ ); };
    std::vector<std::future<void>> futures;
    while (it != algs_.end() && (*it)->l_ < seg.end_) {
        futures.push_back(std::async(std::launch::async, job, *it));
        ++it;
    }
    for (auto& future : futures) future.get();
}

//template<class InputMgr>
void HistPDG::next() {
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

#endif //INFLUENCERS_TRACKING_HIST_PDG_H
