#pragma once

#include "filter_queue.h"
#include "filter_qqhtd_compact.h"

struct QueueQQHTDFilter : QueueFilter {
private:
    void create_fresh_filter(); 

protected:
    size_t n_buckets;
    size_t fingerprint_size;

public:
    QueueQQHTDFilter(size_t memory_size, size_t n_n_buckets, size_t n_fingerprint_size, size_t n_number_filters, size_t n_sliding_window);
    QQHTDCompactFilter Init() const;
    std::string name() const { return "Queueing QQHTD"; };
};