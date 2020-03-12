#pragma once

#include "filter_queue.h"
#include "filter_sq.h"

struct QueueSQFilter : QueueFilter {
protected:
    void create_fresh_filter(); 

    size_t n_buckets;
    uint8_t remainder_bits;
    uint8_t reduced_remainder_bits;

public:
    QueueSQFilter(size_t memory_size, size_t n_n_buckets, uint8_t n_remainder_bits, uint8_t n_reduced_remainder_bits, size_t n_number_filters, size_t n_sliding_window);
    std::string name() const { return "Queueing SQF"; };
};