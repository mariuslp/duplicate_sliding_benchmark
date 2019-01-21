#pragma once

#include <vector>
#include <cmath>

#include "logger.h"

/** Used by the Statistics class*/
struct Event {

	/** Stores the event predicted value */
	bool predicted_value;

	/** Stores the event actual value*/
	bool true_value;

	/** Default constructor */
	Event(): predicted_value(false), true_value(true) { };

	/** Event construcor*/
	Event(const bool actual, const bool predicted) : predicted_value(predicted), true_value(actual) { 
		/**  Creates a new event
		 *   @param actual: real value of the event
		 *   @param predicted: predicted value of the event
		 */
	};
};

/** Statistics class */
struct Statistics {

	/** FP counter */
	uint64_t FP_counter;

	/** FN counter */
	uint64_t FN_counter;

	/** Oh come on */
	uint64_t duplicates_counter;

	uint64_t elements_counter;

	/** Record a new event */
	void addEvent(Event e) {
	   /** Increment counters, according to the event
		*   @param e: New Event object 
		*/
		++elements_counter;

		// If e is a duplicate
		if(e.true_value == true) {
			++duplicates_counter;
		}

		// If e is a FP
		if((e.predicted_value == true) && (e.true_value == false)) {
			++FP_counter;
		}
		// If e is a FN
		if((e.predicted_value == false) && (e.true_value == true)) {
			++FN_counter;
		}
	};

	/** Computes the empirical FPR */
	double getFalsePositiveRate();

	/** Computes the empirical FNR */
	double getFalseNegativeRate();
};
