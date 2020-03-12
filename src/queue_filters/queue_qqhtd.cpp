#include "queue_qqhtd.h"

void QueueQQHTDFilter::create_fresh_filter() {
    queue.push_front(std::make_unique<QQHTDCompactFilter>(memory_per_filter, n_buckets, fingerprint_size));
}

QueueQQHTDFilter::QueueQQHTDFilter(size_t memory_size, 
            size_t n_n_buckets,
            size_t n_fingerprint_size,
            size_t n_number_filters,
            size_t n_sliding_window) : 
        number_filters(n_number_filters), 
        sliding_window(n_sliding_window),
        n_buckets(n_n_buckets), 
        fingerprint_size(n_fingerprint_size) {

    Log("Total memory: ", memory_size, ", n_filters: ", n_number_filters);
    Log("Sliding window ", sliding_window);
    memory_per_filter = memory_size / n_number_filters;

    elements_per_filter = (size_t) ceil((float) sliding_window / number_filters);
    Log("Changing every ", elements_per_filter);
    
    /* populating the filters */
    for(size_t i = 0; i < number_filters; ++i) {
        create_fresh_filter();
    }
    counter = 0;
}

QQHTDCompactFilter QueueQQHTDFilter::Init() const {
    return QQHTDCompactFilter(memory_per_filter, n_buckets, fingerprint_size);
}


void QueueQQHTDFilter::debug() const {
    for(size_t i = 0; i < number_filters; ++i) {
        queue[i]->debug();
    }
}

void QueueQQHTDFilter::Reset() {
    for(size_t i = 0; i < number_filters; ++i) {
        queue[i]->Reset();
        counter = 0;
    }
}