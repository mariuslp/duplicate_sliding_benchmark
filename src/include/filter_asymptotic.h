#pragma once

#include <random>
#include <cassert>

#include "types.h"
#include "filter_generic.h"

/** This Filter does not store any element and simply returns a random answer */
struct AsymptoticFilter : Filter {
private:
	/** Underlying RNG */
	std::mt19937_64 mt;

	/** Bernouilli distribution */
	std::bernoulli_distribution B;

	double hit_probability;

public:
	/** Constructor of the filter */
	AsymptoticFilter(const double p);

	void Reset() override {};

	bool Insert(const Element& e) override { 
		return Lookup(e);
	};

	/** Returns a positive answer with probability p, regardless of the argument */
	bool Lookup(const Element& e) override;

	void debug() const override {};

	std::string name() const override { return "Asymptotic Filter"; };
};
