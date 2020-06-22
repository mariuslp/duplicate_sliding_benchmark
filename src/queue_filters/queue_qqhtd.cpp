#include "queue_qqhtd.h"

void QueueQQHTDFilter::create_fresh_filter() {
    queue.push_front(std::make_unique<QQHTDCompactFilter>(memory_per_filter, n_buckets, fingerprint_size));
    queue[0]->Reset();
}

QueueQQHTDFilter::QueueQQHTDFilter(size_t memory_size, 
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
