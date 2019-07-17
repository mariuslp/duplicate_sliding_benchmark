#pragma once

#include <random>
#include <cassert>

// For debugging purposes only
#include <iostream>

#include "filter_qhtd_compact.h"
#include "types.h"
#include "hash.h"
#include "fastmath.h"

/**
 * TODO
 */
struct QQHTDCompactFilter : QHTDCompactFilter {
	
    QQHTDCompactFilter(const uint64_t memory_size, const size_t n_n_buckets, const size_t n_fingerprint_size);

	bool Insert(const Element& e) override;

	std::string name() const override { return "QQHTD compact"; };

protected:
    bool InsertFingerprintInLastBucket(const uint64_t address, const uint16_t fingerprint);
};
