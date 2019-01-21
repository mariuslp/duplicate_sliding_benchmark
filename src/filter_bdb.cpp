#include "filter_bdb.h"

bDecayingBloomFilter::bDecayingBloomFilter(uint64_t memory_size, uint64_t W) {
	// Ensures counter_size >= 2
	assert(memory_size > W / std::log(2));
	
	window_size = W;

	int counter_size = static_cast<int>(std::ceil(std::sqrt(memory_size * std::log(2) / window_size)));
	// <= 64 because the filter stores values as uint64_t
	assert(counter_size <= 64);

	array_size = memory_size / counter_size;
	bdbf.assign(array_size, 0);

	local_threshold = pow2(counter_size - 1);
	// k = counter size in optimal settings
	k = counter_size;
	iteration_threshold = std::ceil(window_size / (double)(pow2(counter_size - 1) - 1.));
}

bool bDecayingBloomFilter::Lookup(const Element& e) {

	/** Returns true if none of the cells have a value smaller than running_counter */

	for(auto& hash : getHashes(k, e, array_size)) {
		if(bdbf.at(hash) < running_counter) {
			return false;
		}
	}

	return true;
}

bool bDecayingBloomFilter::Insert(const Element& e) {
	bool detected = Lookup(e);

	if(iteration % iteration_threshold == 0) {
		running_counter = (running_counter + 1) % local_threshold;

		if(running_counter == 0) {
			for(auto& cell : bdbf) {
				if(cell <= local_threshold) {
					cell = 0;
				} 
				else {
					cell -= local_threshold;
				}
			}
		}
	}

	assert(window_size / iteration_threshold + running_counter + 1 < pow2(k));
	for(auto& hash : getHashes(k, e, array_size)) {
		bdbf.at(hash) = static_cast<uint64_t>(window_size / iteration_threshold + running_counter + 1);
	}

	++iteration;

	return detected;
}

void bDecayingBloomFilter::Reset() {
	bdbf.assign(array_size, 0);
}

void bDecayingBloomFilter::debug() const {
	for(auto& cell : bdbf) {
		Log(cell);
	}
}
