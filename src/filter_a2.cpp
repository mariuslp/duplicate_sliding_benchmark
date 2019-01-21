#include "filter_a2.h"

A2Filter::A2Filter(const uint64_t memory_size, const float window_FPR) {
	/** Constructor for A^2 filter
	 * @param memory_size: in bits, memory size
	 * @param window_FPR: the desired FPR on the sliding window
	 *	 For precisions, see paper: Aging Bloom Filter with Two Active Buffers for Dynamic Sets
	 */
	m	 = memory_size;
	k	 = std::max(1, static_cast<int>(floor(-log(1 - sqrt(1 - window_FPR)))));
	n_max = static_cast<size_t>(floor((float)m / (float)(2 * k) * log(2)));

	// Create two vectors of m/2 bits each
	// TODO: should have fixed size
	active1.assign(m / 2, 0);
	active2.assign(m / 2, 0);

	n_inserted = 0;
}

bool A2Filter::Lookup(const Element& e) {
	return LookupOneTable(e, active1) || LookupOneTable(e, active2);
}

bool A2Filter::LookupOneTable(const Element& e, const std::vector<bool>& active) const {
	/** Look if one element is in active Bloom Filter
	 * @param e: the element
	 * @param active: the table (bloom filter)
	 * 
	 * @return true if all k hash functions map to cells equal to 1
	 */

	for(auto& hash : getHashes(k, e, active.size())) {
		if(active.at(hash) == 0) {
			return false;
		}
	}

	return true;
}

void A2Filter::InsertOneTable(const Element& e, std::vector<bool>& active) {
	/** Insert element in the specified active Bloom Filter
 	* @param e: the element
 	* @param active: the table (bloom filter)
 	*/

	for(auto& hash: getHashes(k, e, active.size())) {
		active.at(hash) = 1;
	}
}

bool A2Filter::Insert(const Element& e) {

	if(LookupOneTable(e, active1)) {
		return true;
	}

	bool detected = LookupOneTable(e, active2);
	InsertOneTable(e, active1);
	n_inserted += 1;

	if(n_inserted >= n_max) {
		// Exchange both vectors
		std::swap(active1, active2);

		// Reinitialize active1
		active1.assign(m / 2, 0);
		n_inserted = 0;

		// Insert e in active1
		InsertOneTable(e, active1);
		n_inserted += 1; // <--- Doesn't it mean simply n_inserted = 1 ???
	}

	return detected;
}
