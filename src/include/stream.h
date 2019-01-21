#pragma once

#include <random>
#include <unordered_map>
// #include <experimental/filesystem> // Uncomment when clang/g++ have correct support
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

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

private:
	/** Storage of all seen elements for benchmarking purposes. */
	std::unordered_map<Element, bool> elements_seen;
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
	 */
	UniformGenerator(const std::uint8_t uniform_element_size);

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
	RealGenerator(const std::string& filename);

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
