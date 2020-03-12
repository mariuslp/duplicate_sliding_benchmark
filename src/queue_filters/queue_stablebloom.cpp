#include "queue_stablebloom.h"

void QueueStableBloomFilter::create_fresh_filter() {
    queue.push_front(std::make_unique<StableBloomFilter>(memory_per_filter, d, n_hash_functions, max_false_positive));
    queue[0]->Reset();
}

QueueStableBloomFilter::QueueStableBloomFilter(size_t memory_size, 
            uint8_t n_d,
            size_t n_n_hash_functions,
            double n_max_false_positive,
            size_t n_number_filters,
            size_t n_sliding_window) : 
        QueueFilter(memory_size, n_number_filters, n_sliding_window),
        d(n_d), 
        n_hash_functions(n_n_hash_functions),
        max_false_positive(n_max_false_positive) {

    /* populating the filters */
    for(size_t i = 0; i < number_filters; ++i) {
        create_fresh_filter();
    }
}
