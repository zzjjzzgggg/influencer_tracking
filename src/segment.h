/**
 * Copyright (C) by J.Z. 2021-04-19 18:17
 * Distributed under terms of the MIT license.
 */

#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include "stdafx.h"

/**
 * An I-set segment.
 * I-sets belonging to a same segment are same to each other. That is, I_l(e)
 * with start <= l < end are the same.
 */
class Segment {
public:
    int start_, end_;
    ISet iset_;

public:
    Segment();

}; /* Segment */

/**
 * A vector of I-set segments.
 */
class Segments {
public:
    std::vector<Segment> segments_;

public:
    Segments();

}; /* Segments */

#endif /* __SEGMENT_H__ */
