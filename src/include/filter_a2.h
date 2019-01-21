#pragma once

#include <random>
#include <functional>

#include "filter_generic.h"
#include "types.h"
#include "hash.h"

struct A2Filter : Filter {
private:
	size_t m;
	size_t n_max;
	int k;
	
	std::vector<bool> active1;
	std::vector<bool> active2;

	size_t n_inserted;

	bool LookupOneTable(const Element& e, const std::vector<bool>& active) const;
	void InsertOneTable(const Element& e, std::vector<bool>& active);

public:
	A2Filter(const uint64_t memory_size, const float window_FPR);

	void Reset() override {
		active1.assign(m / 2, 0);
		active2.assign(m / 2, 0);
		n_inserted = 0;
	};

	bool Insert(const Element& e) override;

	bool Lookup(const Element& e) override;

	void debug() const override {};

	std::string name() const override { return "A2"; };
};
