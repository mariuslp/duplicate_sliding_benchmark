#include "filter_qhtd_compact.h"

QHTDCompactFilter::QHTDCompactFilter(
	const uint64_t memory_size,
	const size_t n_n_buckets,
	const size_t n_fingerprint_size
) : QHTCompactFilter(memory_size, n_n_buckets, n_fingerprint_size)
{}

bool QHTDCompactFilter::Insert(const Element& e) {

	/** Inserts element e in the filter
	 * @param e
	 * @returns boolean being true if the element was already in the filter
	 */

	auto detected = Lookup(e);
	
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
