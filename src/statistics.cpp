#include "statistics.h"

double Statistics::getFalsePositiveRate() {
	/** Computes the false positive rate 
	 *   @returns The FPR as a double
	 */

	auto unique_counter = elements_counter - duplicates_counter;

	if (unique_counter == 0) {
		// If no unique element were seen, return 0
		return 0;
	}

	double FPR = static_cast<double>(FP_counter) / unique_counter;
	
	if (std::isinf(FPR)) {
		// Upon float overflow, return 0
		return 0;
	}

	return FPR;
}

double Statistics::getFalseNegativeRate() {
	/** Computes the false negative rate 
	 *   @returns The FNR as a double
	*/	

	if (duplicates_counter == 0) {
		return 0;
	}

	double FNR = static_cast<double>(FN_counter) / duplicates_counter;


	if (std::isinf(FNR)) {
		// Upon float overflow, return 0
		return 0;
	}

	return FNR;
}
