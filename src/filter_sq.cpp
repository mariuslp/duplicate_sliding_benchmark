#include "filter_sq.h"
#include "cassert"
#include <iostream>
#include "logger.h"

SQFilter::SQFilter(
	const uint64_t memory_size,
	const uint8_t n_n_buckets,
	const uint8_t n_remainder_bits,
    const uint8_t n_reduced_remainder_bits
) : n_buckets(n_n_buckets), remainder_bits(n_remainder_bits), reduced_remainder_bits(n_reduced_remainder_bits)
{
    fingerprint_size = reduced_remainder_bits + ceil(log2(remainder_bits + 1));

	array_bits = floor(log2((double)memory_size / (double)(n_buckets * fingerprint_size)));

	assert(fingerprint_size <= 8);  // Required because a Bucket is uint8_t
    assert(64 > array_bits + remainder_bits); // Required because we use a 64-bit hash function

	std::random_device rd;
	rng = std::mt19937(rd());

	bucket_selector = std::uniform_int_distribution<size_t>(0, n_buckets - 1);

    auto hash_mask = (1ULL << (remainder_bits + array_bits)) - 1;

    remainder_mask = (1ULL << remainder_bits) - 1;
    reduced_remainder_mask = (1ULL << reduced_remainder_bits) - 1;

    address_mask = hash_mask & ~remainder_mask;

    // Log("Creating SQF, ", memory_size, " bits max, N = ", pow2(array_bits), " k =", n_buckets, ", r = ", unsigned(remainder_bits), " r' = ", unsigned(reduced_remainder_bits));
}

bool SQFilter::InCell(const uint64_t address, const uint8_t fingerprint) const {
	for(Bucket bucket : sqf.at(address)) {
		if(bucket == fingerprint) {
			return true;
		}
	}
	return false;
}

int SQFilter::HammingWeight(const HashValue hash) const {
    int result = 0;
    auto hash_copy = hash;

    while(hash_copy) {
        ++result;
        hash_copy &= hash_copy - 1;
    }

    return result;
}

/** Get the fingerprint of an element
 * @param Element e
 * @return int fingerprint of e
 */
Bucket SQFilter::Fingerprint(HashValue remainder_hash) {

    auto reduced_remainder = remainder_hash & reduced_remainder_mask;
    auto hamming_weight = HammingWeight(remainder_hash);

    Bucket fingerprint = (hamming_weight << reduced_remainder_bits) + reduced_remainder;

	return fingerprint;
}

bool SQFilter::InsertEmpty(const uint64_t address, const uint8_t fingerprint) {
	for(Bucket& bucket : sqf.at(address)) {
		if(bucket == empty_value) {
			bucket = fingerprint;
			return true;
		}
	}
	return false;
}

bool SQFilter::Lookup(const Element& e) {
        auto hash = Hash1(e);

    size_t address = (hash & address_mask) >> remainder_bits;
    auto fingerprint = Fingerprint(hash & remainder_mask);

	return InCell(address, fingerprint);
}

bool SQFilter::Insert(const Element& e) {
        auto detected = Lookup(e);

	if(detected) {
		return detected;
	}

    auto hash = Hash1(e);

    size_t address = (hash & address_mask) >> remainder_bits;
    auto fingerprint = Fingerprint(hash & remainder_mask);

	// First try inserting in empty bucket
	if(InsertEmpty(address, fingerprint)) {
		return detected;
	}

	// No empty bucket, inserting in random bucket (erasing previous content)
	sqf.at(address).at(bucket_selector(rng)) = fingerprint;

	return detected;
}

void SQFilter::Reset() {
	sqf.assign(pow2(array_bits), Buckets(n_buckets, 1));
}

void SQFilter::debug() const {
	for(auto& buckets : sqf) {
		for(auto& bucket : buckets) {
			std::cout << +bucket << ' ';
		}
		std::cout << std::endl;
	}

	std::cout << "========" << std::endl;
}
