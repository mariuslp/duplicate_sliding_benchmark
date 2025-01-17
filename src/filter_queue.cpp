#include "filter_queue.h"
#include "logger.h"

QueueFilter::QueueFilter(size_t memory_size,
            size_t n_number_filters,
            size_t n_sliding_window) :
        number_filters(n_number_filters),
        sliding_window(n_sliding_window) {
    
    Log("Total memory: ", memory_size, ", n_filters: ", n_number_filters);
    Log("Sliding window ", sliding_window);
    memory_per_filter = memory_size / n_number_filters;

    Log("memory per filter, ", memory_per_filter);
    elements_per_filter = (size_t) ceil((float) sliding_window / number_filters);
    Log("Changing every ", elements_per_filter);
    
    counter = 0;
}

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

        // Creates a fresh filter and adds it to the queue
        create_fresh_filter();
        
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