#pragma once

#include "types.h"

/** Generic filter class to serve as a model for filter implementations */
struct Filter {
	/** Reset filter */
	virtual void Reset() = 0;

	/** Insert element. Returns true if the element was already in the filter, false otherwise */
	virtual bool Insert(const Element& e) = 0;

	/** Look element up */
	virtual bool Lookup(const Element& e) = 0;

	// Temporary?
	virtual void debug() const = 0;

	// Desctructor
	virtual ~Filter() {};

	// Filter name
	virtual std::string name() const { return "Generic Filter"; };
};
