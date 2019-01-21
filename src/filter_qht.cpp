#include "filter_qht.h"
#include "cassert"

QHTFilter::QHTFilter(
	const uint64_t memory_size,
	const size_t n_n_buckets,
	const size_t n_fingerprint_size
) : n_buckets(n_n_buckets), fingerprint_size(n_fingerprint_size)
{
	array_size = memory_size / (n_buckets * fingerprint_size);
	assert(array_size > 0);

	assert(fingerprint_size <= 8);

	std::random_device rd;
	rng = std::mt19937(rd());

	bucket_selector = std::uniform_int_distribution<size_t>(0, n_buckets - 1);

	Log("QHT, M = ", memory_size, " N = ", array_size, " k = ", n_buckets, "s = ", fingerprint_size);
}

bool QHTFilter::InCell(const uint64_t address, const uint8_t fingerprint) const {
	for(Bucket bucket : qht.at(address)) {
		if(bucket == fingerprint) {
			return true;
		}
	}
	return false;
}

/** Get the fingerprint of an element
 * 0 is a reserved value and as such cannot be used as a fingerprint
 * For this reason we iterate on hashing until we find a nonzero fingerprint
 * (Averaged number of hashes: 1 + 1/2^fingerprint_size)
 * This configuration makes sure every fingerprint is equiprobable
 * (at the cost of slight computing overhead)
 *
 * @param Element e
 * @return int fingerprint of e
 */
uint8_t QHTFilter::Fingerprint(const Element& e) {
	// Note: the hash must be independent from Hash1 which already provides `address`
	HashValue hash = Hash2(e);

	uint8_t fingerprint = static_cast<uint8_t>(hash % pow2(fingerprint_size));

	int adder = 0;
	// TODO std::hash(hash + ++adder) seems to have poor statistical properties as this loop is run a bit too often to my taste
	while(fingerprint == 0) {
		boost::hash_combine(hash, hash + ++adder);  // adder avoids potential infinite loops with fixed points (such as 11754104648456392440)
		fingerprint = static_cast<uint8_t>(hash % pow2(fingerprint_size));
	}

	return fingerprint;
}

bool QHTFilter::InsertEmpty(const uint64_t address, const uint8_t fingerprint) {
	for(Bucket& bucket : qht.at(address)) {
		if(bucket == 0) {
			bucket = fingerprint;
			return true;
		}
	}
	return false;
}

bool QHTFilter::Lookup(const Element& e) {
	size_t address = Hash1(e) % array_size;
	auto fingerprint = Fingerprint(e);

	return InCell(address, fingerprint);
}

bool QHTFilter::Insert(const Element& e) {
	auto detected = Lookup(e);

	if(detected) {
		return detected;
	}

	size_t address = Hash1(e) % array_size;
	auto fingerprint = Fingerprint(e);

	// First try inserting in empty bucket
	if(InsertEmpty(address, fingerprint)) {
		return detected;
	}

	// No empty bucket, inserting in random bucket (erasing previous content)
	qht.at(address).at(bucket_selector(rng)) = fingerprint;

	return detected;
}

void QHTFilter::Reset() {
	qht.assign(array_size, Buckets(n_buckets, 0));
}

void QHTFilter::debug() const {
	for(auto& buckets : qht) {
		for(auto& bucket : buckets) {
			std::cout << +bucket << ' ';
		}
		std::cout << std::endl;
	}

	std::cout << "========" << std::endl;
}
