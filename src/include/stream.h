#pragma once

#include <random>
#include <unordered_map>
// #include <experimental/filesystem> // Uncomment when clang/g++ have correct support
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <deque>

#include "types.h"
#include "logger.h"

/** Abstract stream generator class */
struct StreamGenerator {

	/** Next output from the generator */
	virtual Element Next() = 0;

	/** Destructor */
	virtual ~StreamGenerator() {};

	/** Inserts the element in the list of elements seen */
	virtual void Insert(const Element& e);

	/** Checks if an element has already been streamed */
	virtual bool Has(const Element& e);

	/** Class name*/
	virtual std::string name() const { return "Generic Stream"; };

	/** 0 for a perfect memory */
	int sliding_window_size;

	/** Counts the number of elements of the stream */
	int stream_counter = 0;

private:
	/** Deletes the last element of the sliding window (both sliding_window and inverted_sliding_window) */
	void PopSlidingWindow();

	/** Storage of all seen elements for benchmarking purposes. */
	std::unordered_map<Element, bool> elements_seen;

	/** Same, but on a sliding window. User choses which mode to use. */
	std::deque<Element> sliding_window;

	/** e maps to a counter, keeping track of the number of occurrences of e in the sliding window. */
	std::unordered_map<Element, int> counter_sliding_window;
};

/** Uniform stream generator */
struct UniformGenerator : StreamGenerator {

	/** Constructor
	 * 
	 * @param uint8_t uniform_element_size
	 * Size of a uniform random element, in bits
	 * Controlling this size, along with the number
	 * of elements in the stream, is controlling 
	 * the proportion of duplicates with the relation
	 *
	 * avg. # of duplicates = n - d + d * ((d-1) / d)^n
	 *
	 * With d = 2^uniform_element_size, n = number of elements in the stream
	 * (https://en.wikipedia.org/wiki/Birthday_problem#Collision_counting)
	 *
	 * Obviously must be <= element_size
	 * 
	 * @param int sliding_window: size of the sliding window. 0 for infinite window
	 */
	UniformGenerator(const std::uint8_t uniform_element_size, const int sliding_window_size_s);

	/** Next output from the generator */
	Element Next() override;

	/** Class name */
	std::string name() const override { return "Uniform Stream"; };


private:
	/** Mask for having the good size */
	std::uint64_t uniform_element_mask;

	/** Internal RNG */
	std::independent_bits_engine<std::mt19937_64, element_size, std::uint64_t> engine;
};

/* Generator based on the contents of a file */
struct RealGenerator : StreamGenerator {

	/** Constructor */
	RealGenerator(const std::string& filename, const int sliding_window_size_s);

	/* Destructor */
	~RealGenerator();

	/** Next output from the generator */
	Element Next() override;

	/** Class name */
	std::string name() const override { return "Real Stream"; };


private:
	/** Internal stream storage. Takes quite a lot of RAM (file is 2.5 GB...) */
	std::ifstream stream_file;
};
