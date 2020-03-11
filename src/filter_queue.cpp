#include "filter_queue.h"
#include <math.h>
#include "logger.h"

bool QueueFilter::Lookup(const Element& e) {
    for(size_t i = 0; i < number_filters; ++i) {
        if(queue[i]->Lookup(e)) {
            return true;
        }
    }

    return false;
}

bool QueueFilter::Insert(const Element& e) {
    bool detected = Lookup(e);

    queue[0]->Insert(e);
    ++counter;

    if(counter == elements_per_filter) {
        queue.pop_back();
        auto new_filter = Init();
        queue.push_front(std::unique_ptr<Filter>(&new_filter);
        queue[0]->Reset();
        counter = 0;
    }

    return detected;
}

void QueueFilter::debug() const {
    for(size_t i = 0; i < number_filters; ++i) {
        queue[i]->debug();
    }
}

void QueueFilter::Reset() {
    for(size_t i = 0; i < number_filters; ++i) {
        queue[i]->Reset();
        counter = 0;
    }
}