#include "filter_cuckoo.h"

#include <iostream>

CuckooFilter::CuckooFilter(
		const uint64_t memory_size,
		const uint8_t  n_fingerprint_size,
		const uint64_t n_n_buckets
	) : fingerprint_size(n_fingerprint_size), n_buckets(n_n_buckets) 
	{

	assert(fingerprint_size <= 8);  // Fingerprints are stored in uint8

	size_t array_size_bits = static_cast<size_t>(std::floor(std::log2(memory_size / (n_buckets * pow2(fingerprint_size)))));
	array_size = pow2(array_size_bits);
	
	cuckooFilter.assign(array_size, Buckets(n_buckets, 0));

	std::random_device rd;
	rng = std::mt19937(rd());

	address_selector = std::uniform_int_distribution<size_t>(0, 1);
	bucket_selector = std::uniform_int_distribution<size_t>(0, n_buckets - 1);
}

uint8_t CuckooFilter::Fingerprint(const Element& e) const {

	/** Hashes are too big, we only store a small value.
	 * Reducing using modulo is not perfect but good enough
	 * 
	 * 0 is a reserved value for unused buckets, therefore should not be used as a fingerprint value
	 * Even though not specified in the paper, this is how the paper implementation dealt with it
	 * 
	 * @param Element the element we want to fingerprint
	 */

	HashValue hash = Hash2(e);
	uint8_t fingerprint = static_cast<uint8_t>(hash % pow2(fingerprint_size));
	
	return fingerprint + (fingerprint == 0);
}

bool CuckooFilter::InCell(const uint64_t address, const uint8_t fingerprint) const {

	/** Given a cell, looks in all buckets whether one of them contains the fingerprint.
	 * @param address the cuckooFilter cell's address to look at
	 * @param fingerprint the fingerprint we are looking for
	 * 
	 * @returns bool
	 */

	for(uint8_t bucket : cuckooFilter.at(address)) {
		if(bucket == fingerprint) {
			return true;
		}
	}

	return false;
}

bool CuckooFilter::InsertEmpty(const uint64_t address, const uint8_t fingerprint) {

	/** Insert fingerprint at given address if one of the buckets is empty
	 *
	 * @param address
	 * @param fingerprint
	 * 
	 * @returns true if the element has been inserted.
	 */

	for(uint8_t& bucket : cuckooFilter.at(address)) {
		if(bucket == 0) {
			bucket = fingerprint;
			return true;
		}
	}
	return false;
}

bool CuckooFilter::Lookup(const Element& e) {

	/** An element can be stored in 2 cells.
	 * We check if the element is in one of the buckets of these 2 cells.
	 * @param Element
	 * @returns bool
	 */

	uint8_t fingerprint = Fingerprint(e);
	size_t  address1 = Hash1(e) % array_size;
	size_t  address2 = address1 ^ (Hash1(fingerprint) % array_size);

	return InCell(address1, fingerprint) || InCell(address2, fingerprint);
}

bool CuckooFilter::Insert(const Element& e) {
	bool detected = Lookup(e);

	uint8_t fingerprint = Fingerprint(e);
	size_t  address1 = Hash1(e) % array_size;
	size_t  address2 = address1 ^ (Hash1(fingerprint) % array_size);

	// Try and insert in empty bucket
	// Will only be inserted once thanks to short-circuit evaluation
	if(InsertEmpty(address1, fingerprint) || InsertEmpty(address2, fingerprint)) {
		return detected;
	}

	// No empty bucket, enter swapping process
	size_t address = address_selector(rng) ? address1 : address2;

	for(size_t n = 0; n < maxRelocations; ++n) {
		// Select random bucket in cell
		int bucket_indice = bucket_selector(rng);

		// Swap fingerprint to insert with fingerprint in bucket
		std::swap(fingerprint, cuckooFilter.at(address).at(bucket_indice));

		// Get the alternative address for new fingerprint to insert
		address = address ^ (Hash1(fingerprint) % array_size);

		if(InsertEmpty(address, fingerprint)) {
			return detected;
		}
	}
	
	// Swapping process has taken more than maxRelocations steps, filter is probably full
	// Dropping current element
	return detected;
	
}

void CuckooFilter::Reset() {
	cuckooFilter.assign(array_size, Buckets(n_buckets, 0));
}

void CuckooFilter::debug() const {
	for(auto& buckets : cuckooFilter) {
		for(auto& bucket : buckets) {
			Log(bucket);
		}
		Log("-");
	}
}
