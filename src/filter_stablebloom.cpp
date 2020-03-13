#include "filter_stablebloom.h"

StableBloomFilter::StableBloomFilter(const uint64_t memory_size, const uint8_t d, const size_t n_hash_functions, const double max_false_positive) {

	assert(d <= 8);

	bits_per_cell = d;
	array_size    = memory_size / d;
	k             = n_hash_functions;
	fp_max        = max_false_positive;

	// Log((size_t) bits_per_cell, " ", array_size, " ", memory_size, k);

	stableFilter.assign(array_size, 0);

	max_value = pow2(d) - 1;

	decaying_bits = std::min(
				array_size, 
				static_cast<size_t>(
					1. / ((1. / std::pow(1 - std::pow(fp_max, 1. / k), 1. / max_value) - 1) * (1. / k - 1. / array_size))
				)
			);

	if(decaying_bits == array_size) {
		Log("Actually constructing a decaying bloom filter");
	}

	std::random_device rd;
	rng = std::mt19937(rd());

	random_selector = std::uniform_int_distribution<size_t>(0, array_size - 1);
}

bool StableBloomFilter::Lookup(const Element& e) {
	for(auto& hash: getHashes(k, e, array_size)) {
		if(stableFilter.at(hash) > 0) {
			return true;
		}
	}
	return false;
}

bool StableBloomFilter::Insert(const Element& e) {
	bool detected = Lookup(e);

	// Decrease P cells by 1
	// Should be P random cells but original paper says P consecutive cells are ok
	size_t random_start = random_selector(rng);

	for(size_t i = 0; i < decaying_bits; ++i) {
		if(stableFilter.at((random_start + i) % array_size) > 0) {
			--stableFilter.at((random_start + i) % array_size);
		}
	}

	// Insert element
	for(auto& hash: getHashes(k, e, array_size)) {
		stableFilter.at(hash) = max_value;
	}

	return detected;
}

void StableBloomFilter::Reset() {
	stableFilter.assign(array_size, 0);
}

void StableBloomFilter::debug() const {
	for (auto& e : stableFilter) {
		log(e);
	}
}
