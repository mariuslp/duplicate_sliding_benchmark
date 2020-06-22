#pragma once

#include <deque>
#include <math.h>
#include <memory>

#include "filter_generic.h"
#include "types.h"
#include "filter_qqhtd_compact.h"

struct QueueFilter : Filter {
protected:
    virtual void create_fresh_filter() = 0;

    std::deque<std::unique_ptr<Filter>> queue;
    size_t counter;
    size_t number_filters;
    size_t sliding_window;
    size_t elements_per_filter;
    size_t memory_per_filter;

public:
    QueueFilter(size_t memory_size, size_t n_number_filters, size_t n_sliding_window);
    bool Insert(const Element& e);
    bool Lookup(const Element& e);
    virtual void debug() const;
    virtual void Reset();
};