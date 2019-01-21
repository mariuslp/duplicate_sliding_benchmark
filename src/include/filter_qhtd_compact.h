#pragma once

#include <random>
#include <cassert>

// For debugging purposes only
#include <iostream>

#include "filter_qht_compact.h"
#include "types.h"
#include "hash.h"
#include "fastmath.h"

/**
 * Space-efficient QHTD
 * Same as QHTFilter but space-efficient, with difference that duplicates are stored anyway
 */
struct QHTDCompactFilter : QHTCompactFilter {
	
	QHTDCompactFilter(const uint64_t memory_size, const size_t n_n_buckets, const size_t n_fingerprint_size);

	bool Insert(const Element& e) override;

	std::string name() const override { return "QHTD compact"; };

};
