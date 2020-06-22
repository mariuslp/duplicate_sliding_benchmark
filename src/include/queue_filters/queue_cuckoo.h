#pragma once

#include "filter_queue.h"
#include "filter_cuckoo.h"

struct QueueCuckooFilter : QueueFilter {
protected:
    void create_fresh_filter(); 

    size_t n_buckets;
    size_t fingerprint_size;

public:
    QueueCuckooFilter(size_t memory_size, size_t n_n_buckets, size_t n_fingerprint_size, size_t n_number_filters, size_t n_sliding_window);
    std::string name() const { return "Queueing Cuckoo"; };
};