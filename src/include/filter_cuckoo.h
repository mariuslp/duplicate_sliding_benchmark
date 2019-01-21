#pragma once

#include <cmath>
#include <cassert>
#include <random>

#include "fastmath.h"
#include "filter_generic.h"
#include "hash.h"
#include "logger.h"

/** Cuckoo Filter
 * Not memory efficient, no semi-sorting
 * 
 * Implementation of "Cuckoo Filter: Practically Better Than Bloom"
 * Our main array is constituted of cells, each cell has a definite number of buckets
 * Each bucket can store one fingerprint
 * (this denomination changes slightly from the paper, in which a bucket is what we call a cell) 
 */
struct CuckooFilter : Filter {
private:
	/** Size of a fingerprint sotred in a bucket */
	uint8_t  fingerprint_size;

	/** Number of cells in the cuckoo filter */
	uint64_t array_size;

	/** Number of buckets per cell */
	uint64_t n_buckets;

	/* The memory array of our filter */
	std::vector<Buckets> cuckooFilter;

	// Random number generator
	std::mt19937 rng;

	// Random generator for bucket selection
	std::uniform_int_distribution<size_t> bucket_selector;

	// Random generator for address selection
	std::uniform_int_distribution<size_t> address_selector;

    // Paper implementation uses 500, but we have to deal with a 100% saturated filter
	const uint64_t maxRelocations = 50;

	// To be documented
	bool    InCell(const uint64_t address, const uint8_t fingerprint) const;

	// To be documented
	uint8_t Fingerprint(const Element& e) const;

	// To be documented
	bool    InsertEmpty(const uint64_t address, const uint8_t fingerprint);

public:
	CuckooFilter(const uint64_t memory_size, const uint8_t fingerprint_size, const uint64_t n_buckets);

	void Reset() override;

	bool Insert(const Element& e) override;

	bool Lookup(const Element& e) override;

	void debug() const override;

	~CuckooFilter() override {};

	std::string name() const override { return "Cuckoo"; };

};
