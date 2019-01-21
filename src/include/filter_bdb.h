#pragma once

#include <cassert>
#include <functional>
#include <math.h>

#include "fastmath.h"
#include "filter_generic.h"
#include "types.h"
#include "hash.h"
#include "logger.h"

/** Block-decaying bloom filter
 * Not memory optimized
 * Reference paper:
 * Improved Approximate Detection of Duplicates for Data Streams Over Sliding Windows 
 * 
 * Implements optimized version with delayed decreasing described in section 5.5 of the paper */
struct bDecayingBloomFilter : Filter {
private:
	// T in the paper
	uint64_t iteration_threshold;

	// Th in the paper
	size_t   local_threshold;

	// VT in the paper
	size_t   running_counter = 0;

	// m in the paper
	size_t   array_size;

	// W in the paper
	size_t   window_size;

	int      k;
	uint64_t iteration = 0;

	std::vector<uint64_t> bdbf;

public:
	bDecayingBloomFilter(uint64_t memory_size, uint64_t W);
	
	bool Insert(const Element& e) override;
	
    	bool Lookup(const Element& e) override;
	
    	void Reset() override;

    	void debug() const override;

	std::string name() const override { return "bDecayingBF"; };
};
