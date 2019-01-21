#pragma once

#include <cassert>
#include <random>
#include <functional>

#include "filter_generic.h"
#include "types.h"
#include "hash.h"
#include "logger.h"
#include "fastmath.h"

/** Stable Bloom Filter of 8 bits per cell max
 *  Definitely not memory efficient...
 */
struct StableBloomFilter : Filter {
private:
	// d in the paper
	uint8_t  bits_per_cell = 0;

	// To be documented
	uint64_t max_value = 0;

	// M/d in the paper
	size_t   array_size = 0;

	// P in the paper
	size_t   decaying_bits = 0;

	// To be documented
	uint64_t k;

	// To be documented
	double   fp_max;

	// Random number generator
	std::mt19937 rng;

	// To be documented
	std::uniform_int_distribution<size_t> random_selector;

	// To be documented
	std::vector<int> stableFilter;

public:
	StableBloomFilter(const uint64_t memory_size, const uint8_t d, const size_t n_hash_functions, const double max_false_positive);

	bool Insert(const Element& e) override;

	bool Lookup(const Element& e) override;

	void Reset() override;

	void debug() const override;

	std::string name() const override { return "SBF"; };
};
