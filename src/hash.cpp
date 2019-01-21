#include "hash.h"

HashValue Hash3(const HashValue t) {
	/** A hash function that can be called several times.
	 * Useful for QHT
	 * 
	 * @param t: object to be hashed
	 * @returns HashValue
	 */
	return std::hash<HashValue>{}(t);
}