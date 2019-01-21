#include "filter_asymptotic.h"

AsymptoticFilter::AsymptoticFilter(const double p) {
	/** Constructor for the AsymptoticFilter
	 *   @param p: Probability to provide a positive answer
	 */

 	assert(p >= 0);
	assert(p <= 1);

	hit_probability = p;

	std::random_device rd;
	mt = std::mt19937_64(rd());

	B = std::bernoulli_distribution(hit_probability);
}

bool AsymptoticFilter::Lookup(const Element& e) {
	/** Returns true or false with probability p, regardless of the
	 * provided element
	 *   @param e: Element to be looked up
	 *   @returns A boolean indicating whether the filter hits or misses
	 */

	UNUSED(e);

	return B(mt);
}
