#pragma once

#include <random>
#include <cassert>

// For debugging purposes only
#include <iostream>

#include "filter_generic.h"
#include "types.h"
#include "hash.h"
#include "fastmath.h"
#include <cmath>

struct SQFilter : Filter {
protected:
	size_t array_bits;
	uint64_t n_buckets;
    uint8_t remainder_bits;
    uint8_t reduced_remainder_bits;
    uint8_t fingerprint_size;

    size_t address_mask;
    size_t remainder_mask;
    size_t reduced_remainder_mask;

    // empty_value is a value never used by the filter signature
    // and as such can be used as an "empty" marker 
    Bucket empty_value = 1;

	std::mt19937 rng;
	std::uniform_int_distribution<size_t> bucket_selector;

	std::vector<Buckets> sqf;

	bool InCell(const uint64_t address, const uint8_t fingerprint) const;

	Bucket Fingerprint(HashValue remainder_hash);

	bool InsertEmpty(const uint64_t address, const uint8_t fingerprint);

    int HammingWeight(const HashValue hash) const;

public:
	SQFilter(const uint64_t memory_size, const uint8_t n_n_buckets, const uint8_t remainder_bits, const uint8_t reduced_remainder_bits);

	void Reset() override;

	bool Lookup(const Element& e) override;

	bool Insert(const Element& e) override;

	void debug() const override;

	std::string name() const override { return "SQF"; };
};
