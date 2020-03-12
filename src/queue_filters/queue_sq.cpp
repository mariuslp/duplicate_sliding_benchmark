#include "queue_sq.h"

void QueueSQFilter::create_fresh_filter() {
    queue.push_front(std::make_unique<SQFilter>(memory_per_filter, n_buckets, remainder_bits, reduced_remainder_bits));
    queue[0]->Reset();
}

QueueSQFilter::QueueSQFilter(size_t memory_size, 
            size_t n_n_buckets,
            uint8_t n_remainder_bits,
            uint8_t n_reduced_remainder_bits,
            size_t n_number_filters,
            size_t n_sliding_window) : 
        QueueFilter(memory_size, n_number_filters, n_sliding_window),
        n_buckets(n_n_buckets), 
        remainder_bits(n_remainder_bits),
        reduced_remainder_bits(n_reduced_remainder_bits) {
    
    /* populating the filters */
    for(size_t i = 0; i < number_filters; ++i) {
        create_fresh_filter();
    }
}
