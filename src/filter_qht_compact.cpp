#include "filter_qht_compact.h"

QHTCompactFilter::QHTCompactFilter(
	const uint64_t memory_size,
	const size_t n_n_buckets,
	const size_t n_fingerprint_size
) : QHTFilter(memory_size, n_n_buckets, n_fingerprint_size)
{
        //n_buckets = n_n_buckets;
        //fingerprint_size = n_fingerprint_size;
	n_cells = memory_size / (n_buckets * fingerprint_size);
	assert(n_cells > 0);
}

uint16_t QHTCompactFilter::GetFingerprintFromBucket(const uint64_t address, const size_t bucket_number) const {

	/**
	 * All bits are stored in sequence.
	 * One cell has n_buckets buckets, each containing fingerprint_size (f_s) bits.
	 * The f_s bits of a fingerprint are stored consecutively, the buckets of a cell too.
	 * Hence the computation of the offset for a given bucket of a given cell.
	 * @param address: to be documented
	 * @param bucket_number: to be documented
	 * @returns to be documented
	 */

	uint16_t fingerprint = 0;

	// The cell `address` starts here
	auto offset = address * n_buckets * fingerprint_size;

	// We add the offset of the number of buckets we want
	offset += bucket_number * fingerprint_size;

	// We rebuild the fingerprint bit per bit
	// Most significant bit has the lowest index
	for(size_t i = 0; i < fingerprint_size; ++i) {
		fingerprint = (fingerprint << 1) + qht.at(offset + i);
	}

	return fingerprint;
}


bool QHTCompactFilter::InsertFingerprintInBucket(const uint64_t address, const size_t bucket_number, const uint16_t fingerprint) {
	
	/** Takes a fingerprint, and inserts it in the given bucket number of a given cell (address)
	 * @param address
	 * @param bucket_number: int in 0..bucket_number - 1
	 * @param fingerprint
	 * @returns true
	 */

	auto offset = address * n_buckets * fingerprint_size + bucket_number * fingerprint_size;

	auto bits_to_insert = fingerprint;
	// We store bits in the allocated slots
	// Most significant bit has the smallest index
	for(size_t i = 0; i < fingerprint_size; ++i) {
		qht.at(offset + (fingerprint_size - i - 1)) = bits_to_insert & 0b1;
		bits_to_insert = bits_to_insert >> 1;
	}

	return true;
}

bool QHTCompactFilter::InCell(const uint64_t address, const uint16_t fingerprint) const {

	/** Return true if a fingerprint is in one of the buckets of a given cell (address)
	 * @param address
	 * @param fingerprint
	 * @returns boolean
	 */

	for(size_t i = 0; i < n_buckets; ++i) {
		if(GetFingerprintFromBucket(address, i) == fingerprint) {
			return true;
		}
	}

	return false;
}

bool QHTCompactFilter::InsertEmpty(const uint64_t address, const uint16_t fingerprint) {

	/** Inserts fingerprint in an empty bucket of the cell (address), if such bucket exists
	 * @param address
	 * @param fingerprint
     * @returns true if element has been inserted, false otherwise
	 */

	for(size_t i = 0; i < n_buckets; ++i) {
		if(GetFingerprintFromBucket(address, i) == 0) {
			InsertFingerprintInBucket(address, i, fingerprint);
			return true;
		}
	}

	return false;
}

bool QHTCompactFilter::Lookup(const Element& e) {

	/** Returns true if the element e is detected inside the filter
     * @param e
	 * @returns boolean
	 */

	size_t address = Hash1(e) % n_cells;
	auto fingerprint = Fingerprint(e);

	return InCell(address, fingerprint);
}

bool QHTCompactFilter::Insert(const Element& e) {

	/** Inserts element e in the filter if not already present
	 * @param e
	 * @returns boolean being true if the element was already in the filter
	 */

	auto detected = Lookup(e);
        
	// Does not insert if the element is already present in the filter
	if(detected) {
		return detected;
	}

	size_t address = Hash1(e) % n_cells;
	auto fingerprint = Fingerprint(e);

	// First try inserting in empty bucket
	if(InsertEmpty(address, fingerprint)) {
		return detected;
	}

	// No empty bucket, inserting in random bucket (erasing previous content)
	InsertFingerprintInBucket(address, bucket_selector(rng), fingerprint);

	return detected;
}

void QHTCompactFilter::Reset() {
	qht.assign(n_cells * n_buckets * fingerprint_size, 0);
}

void QHTCompactFilter::debug() const {
	size_t bucket_counter = 0;
	size_t fingerprint_counter = 0;

	for(uint64_t i = 0; i < qht.size(); ++i) {
		std::cout << qht.at(i);

		if (++fingerprint_counter == fingerprint_size) {
			fingerprint_counter = 0;
			std::cout << ' ';

			if(++bucket_counter == n_buckets) {
				bucket_counter = 0;
				std::cout << std::endl;
			}
		}
	}

	std::cout << "========" << std::endl;
}
