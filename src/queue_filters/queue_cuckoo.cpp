#include "queue_cuckoo.h"

void QueueCuckooFilter::create_fresh_filter() {
    queue.push_front(std::make_unique<CuckooFilter>(memory_per_filter, fingerprint_size, n_buckets));
    queue[0]->Reset();
}

QueueCuckooFilter::QueueCuckooFilter(size_t memory_size, 
            size_t n_n_buckets,
            size_t n_fingerprint_size,
            size_t n_number_filters,
            size_t n_sliding_window) : 
        QueueFilter(memory_size, n_number_filters, n_sliding_window),
        n_buckets(n_n_buckets), 
        fingerprint_size(n_fingerprint_size) {

    /* populating the filters */
    for(size_t i = 0; i < number_filters; ++i) {
        create_fresh_filter();
    }
}
