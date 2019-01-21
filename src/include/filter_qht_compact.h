#pragma once

#include <random>

// For debugging purposes only
#include <iostream>

#include "filter_qht.h"
#include "types.h"
#include "hash.h"
#include "fastmath.h"

/**
 * Space-efficient QHT
 * Same as QHTFilter but space-efficient
 */
struct QHTCompactFilter : QHTFilter {
	
	QHTCompactFilter(const uint64_t memory_size, const size_t n_n_buckets, const size_t n_fingerprint_size);

	void Reset() override;

	bool Lookup(const Element& e) override;

	bool Insert(const Element& e) override;

	void debug() const override;

	std::string name() const override { return "QHT compact"; };

protected:
	/* QHT storage. We only use an array of bits for which we precisely control the size.
	 * Buckets (and cells) are emulated
	 */
	std::vector<bool> qht;

	/** Shortcut for the number of cells in the filter (N) */
	size_t n_cells;

	/* Check if `fingerprint` is one of the buckets of the cell `address` */
	bool InCell(const uint64_t address, const uint8_t fingerprint) const;

	/* Insert fingerprint in one of the empty buckets of address. Returns true, false if no empty bucket */
	bool InsertEmpty(const uint64_t address, const uint8_t fingerprint);

	/* Insert fingerprint in random bucket of cell address */
	bool InsertFingerprintInBucket(const uint64_t address, const size_t bucket_number, const uint8_t fingerprint);

	/* Retreive a fingerprint from bucket `bucket_number` of cell `address` */
	uint8_t GetFingerprintFromBucket(const uint64_t address, const size_t bucket_number) const;
};
