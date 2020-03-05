#pragma once

#include <deque>

#include "filter_generic.h"
#include "types.h"
#include "filter_qqhtd_compact.h"

struct QueueFilter : Filter {
protected:
    std::deque<QQHTDCompactFilter> queue;
    size_t counter;
    size_t number_filters;
    size_t sliding_window;
    size_t elements_per_filter;
    size_t memory_per_filter;
    size_t n_buckets;
    size_t fingerprint_size;

public:
    QueueFilter(size_t memory_size, size_t n_n_buckets, size_t n_fingerprint_size, size_t n_number_filters, size_t n_sliding_window);
    bool Insert(const Element& e);
    bool Lookup(const Element& e);
    void debug() const;
    void Reset();
    std::string name() const { return "Queue QQHTD Filter"; };
};