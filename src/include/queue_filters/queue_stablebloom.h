#pragma once

#include "filter_queue.h"
#include "filter_stablebloom.h"

struct QueueStableBloomFilter : QueueFilter {
protected:
    void create_fresh_filter(); 

    uint8_t d;
    size_t n_hash_functions;
    double max_false_positive;

public:
    QueueStableBloomFilter(size_t memory_size, uint8_t d, size_t n_hash_functions, double max_false_positive, size_t n_number_filters, size_t n_sliding_window);
    std::string name() const { return "Queueing StableBloom"; };
};