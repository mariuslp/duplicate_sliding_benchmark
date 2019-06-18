#include "stream.h"
#include "logger.h"

// Generic methods

void StreamGenerator::PopSlidingWindow() {
	Element e_to_remove = sliding_window.back();
	sliding_window.pop_back();

	if(--counter_sliding_window[e_to_remove] == 0) {  // Delete the entry: saves memory
		counter_sliding_window.erase(e_to_remove);
	}
}


void StreamGenerator::Insert(const Element& e) {
	if(sliding_window_size == 0) {
		// Log("no sliding!");
		elements_seen[e] = true;
	}
	else {
		// Log("sliding!");
		sliding_window.push_front(e);
		++counter_sliding_window[e];

		if(counter > sliding_window_size) {
			PopSlidingWindow();
		}
	}
}

bool StreamGenerator::Has(const Element& e) {
	if(sliding_window_size == 0) {
		return elements_seen[e];
	}
	else {
		return counter_sliding_window[e] > 0;

		// for (int i = 0; i < sliding_window.size(); ++i) {
		// 	if(sliding_window.at(i) == e) {
		// 		return true;
		// 	}
		// }

		// return false;
	}
}

// UniformGenerator
UniformGenerator::UniformGenerator(
	const uint8_t uniform_element_size,
	const int sliding_window_size_s
) {
	sliding_window_size = sliding_window_size_s;
	/** Initialisation of the uniform random element generator */
	std::random_device rd;
	engine = std::independent_bits_engine<std::mt19937_64, element_size, std::uint64_t>();

	assert(uniform_element_size < element_size);
	assert(uniform_element_size < 64); 
	uniform_element_mask = (1LL << uniform_element_size) - 1;
}

Element UniformGenerator::Next() {
	/** @returns The next random element in the stream */
	return Element(uniform_element_mask & engine());
}

// RealGenerator
RealGenerator::RealGenerator(
	const std::string& filename,
	const int sliding_window_size_s
) {
	sliding_window_size = sliding_window_size_s;
	/** Constructor */

	/* Clang/G++ have poor support for now
	if (!std::experimental::filesystem::exists(filename)) {
		// The data file does not exist
		Log("\t[Error]\tThe file '", filename, "' does not exist. Please refer to the README.");
		abort();
	}
	*/

	stream_file = std::ifstream(filename);

	if(!stream_file.is_open()) {
		Log("\t[Error]\tThe file '", filename, "' cannot be read. Please check access rights.");
		abort();
	}
}

RealGenerator::~RealGenerator() {
	/** Destructor */
	
	stream_file.close();
}

Element RealGenerator::Next() {

	std::string line;
	std::stringstream ss;
	uint64_t hash_value; // Hashes are xxhash64 so hold in 64 bits
	
	auto has_value = false;

	while (std::getline(stream_file, line)) {
		if (line.empty()) {
			// The file may contain empty lines that we ignore
			continue;
		}
		// Otherwise it contains hex-formatted data
		ss << std::hex << line;
		has_value = true;
		break;
	} 
	
	if (!has_value) {
		// The file has ended or no hash value could be found, return 0
		throw "Error while reading stream";
	}
	
	// Otherwise return the current hash value
	ss >> hash_value;
	return Element(hash_value);
}
