#include "filter_qqhtd_compact.h"

QQHTDCompactFilter::QQHTDCompactFilter(const uint64_t memory_size, const size_t n_n_buckets, const size_t n_fingerprint_size): QHTDCompactFilter(memory_size, n_n_buckets, n_fingerprint_size) {
}

bool QQHTDCompactFilter::Insert(const Element& e) {

	/** Inserts element e in the filter
	 * @param e
	 * @returns boolean being true if the element was already in the filter
	 */

	auto detected = Lookup(e);
	
    size_t address = Hash1(e) % n_cells;
	auto fingerprint = Fingerprint(e);


	// Inserting in last bucket (popping content of first bucket, FIFO style)
	InsertFingerprintInLastBucket(address, fingerprint);

	return detected;
}


bool QQHTDCompactFilter::InsertFingerprintInLastBucket(const uint64_t address, const uint16_t fingerprint) {
    /**
     * In QQHT, buckets behave like a queue. Therefore each new element is inserted at the end of the queue
     * Using a linked list would use additional bits of data (for storing pointers with history).
     * Therefore we manually reorder the buckets at every insertion.
     * 
     * @param address
     * @param fingerprint
     * @returns true
     */
    for(size_t i = 0; i < n_buckets - 1; ++i) {
        InsertFingerprintInBucket(address, i, GetFingerprintFromBucket(address, i + 1));
    }

    InsertFingerprintInBucket(address, n_buckets - 1, fingerprint);

    return true;
}