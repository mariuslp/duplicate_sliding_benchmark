#pragma once

#include <random>
#include <cassert>

// For debugging purposes only
#include <iostream>
#include "logger.h"

#include "filter_generic.h"
#include "types.h"
#include "hash.h"
#include "fastmath.h"

struct QHTFilter : Filter {
protected:
	size_t array_size;
	size_t n_buckets;
	size_t fingerprint_size;

	std::mt19937 rng;
	std::uniform_int_distribution<size_t> bucket_selector;

	std::vector<Buckets> qht;

	bool InCell(const uint64_t address, const uint16_t fingerprint) const;

	uint16_t Fingerprint(const Element& e);

	bool InsertEmpty(const uint64_t address, const uint16_t fingerprint);

public:
	QHTFilter(const uint64_t memory_size, const size_t n_n_buckets, const size_t n_fingerprint_size);

	void Reset() override;

	bool Lookup(const Element& e) override;

	bool Insert(const Element& e) override;

	void debug() const override;

	std::string name() const override { return "QHT"; };
};
