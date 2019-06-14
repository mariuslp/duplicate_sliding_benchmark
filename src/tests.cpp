#include "tests.h"
#include <cmath>
#include <sys/time.h>
#include <iostream>

double round_dec(double value) {
	return std::round(value * 100) / 100;
}


void run_tests() {
	/**
	* Runs tests
	*/

	Log("\t[Test]\t", "Running tests.");

	// Setup
	// -----

	// Length of the test stream
	const auto stream_length	= 1000000; //10000000;

	// Number of tested filters 
	const auto num_filters 		= 5;
	
	// Number of streams
	const auto num_streams 		= 2;

        // Number of memory variations for each filters
	const auto num_multipliers 	= 1; //9;

	// Amount of memory available to filters
	const auto memory 		= 1000000; // 10e6

	// Memory variations for each filter
	double memory_multipliers [num_multipliers] = {1}; //{200, 100, 60, 30, 10, 1, 0.1, 0.01, 0.001};

	// duh
	int sliding_window_size = 100000;

	// Collection of streams
	std::vector<std::unique_ptr<StreamGenerator> > streams;

	// Collection of filters
	std::vector<std::unique_ptr<Filter> > filters;

	auto t0 = std::chrono::high_resolution_clock::now();

	// Assemble the stream collection
	// streams.push_back(std::make_unique<UniformGenerator>(30, sliding_window_size));
	//streams.push_back(std::make_unique<UniformGenerator>(27, sliding_window_size));
	streams.push_back(std::make_unique<UniformGenerator>(26, sliding_window_size));
	streams.push_back(std::make_unique<RealGenerator>("hash_sorted.dat", sliding_window_size));
	
	double stream_duplicates [num_streams];

	auto t1 = std::chrono::high_resolution_clock::now();

	for(auto multiplier : memory_multipliers) {
		uint64_t memsize = multiplier * memory;

                filters.push_back(std::make_unique<SQFilter>(memsize, 1, 2, 1));
                //filters.push_back(std::make_unique<QHTFilter>(memsize, 1, 3));
                filters.push_back(std::make_unique<QHTCompactFilter>(memsize, 1, 3));
                //filters.push_back(std::make_unique<QQHTDCompactFilter>(memsize, 1, 3));
                filters.push_back(std::make_unique<CuckooFilter>(memsize, 3, 1));
                filters.push_back(std::make_unique<StableBloomFilter>(memsize, 2, 2, 0.02));
                filters.push_back(std::make_unique<A2Filter>(memsize, 0.1));
                //filters.push_back(std::make_unique<bDecayingBloomFilter>(memsize, 6000));
      }

        assert(filters.size() == num_filters * num_multipliers);
        assert(streams.size() == num_streams);

        // Result, for each filter of each stream (FPR and FNR)
	double results [num_streams][num_multipliers][num_filters][2];
        std::chrono::nanoseconds time_insertion [num_streams][num_multipliers][num_filters];

	auto t2 = std::chrono::high_resolution_clock::now();

	// Begin test
	// ----------

	// First Test : average insert time per filter
	// ----------
        /*
	timeval time;

	 for(auto& filter: filters) {
                filter->Reset();
                Log("filter ", filter->name());
	 	gettimeofday(&time, NULL);
	 	auto t0 = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;

	 	for(auto i = 0; i < stream_length; ++i) {
	 		auto e = streams.at(0)->Next();
	 		filter->Insert(e);
	 	}

	 	gettimeofday(&time, NULL);
	 	auto t1 = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;

	 	std::cout << (t1 - t0) / (double)stream_length << std::endl;
	 }
*/

	// Second test : error rate per filter
	// -----------
	size_t s = 0;
	while (!streams.empty()) {
		// For each stream
		auto& stream = streams.back();

		Log("\t[Test] [Stream] New stream: ", stream->name());

		// Create the stats data for each filter
		std::map<uint64_t, std::unique_ptr<Statistics>> stats;

		size_t duplicate_count = 0; // Small helper

		// Reset all filters
		for (auto& filter : filters) {
			filter->Reset();
		}

		// Reset all stats
		uint64_t u = 0;
		for (auto& filter : filters) {
			stats[u++] = std::make_unique<Statistics>();
		}

		// Generate stream
		for(auto i = 0; i < stream_length; ++i) {

			// Get the next element
			auto e = stream->Next();
			auto is_duplicate = stream->Has(e);

			if(is_duplicate) {
				++duplicate_count;
			}

			u = 0;
			for (auto& filter : filters) {
                                auto begin_nanosec = std::chrono::high_resolution_clock::now();
				// Insert element in all filters
				auto detected = filter->Insert(e);
                                
                                auto end_nanosec = std::chrono::high_resolution_clock::now();

                                // Store time needed
                                time_insertion[s][u / num_filters][u % num_filters] += end_nanosec - begin_nanosec;
				// Update filter's stats
				stats[u++]->addEvent(Event(is_duplicate, detected));
			}

			stream->Insert(e);
		}

		Log("\t[Test] [Stream statistics]\tDuplicate count: ", duplicate_count, "/", stream_length, " (", 100. * duplicate_count / stream_length, "%)");
	
		stream_duplicates[s] = 100. * duplicate_count / stream_length;

		// Print stats for stream
		// Stats are rounded to the 4th decimal
		
		u = 0;
		for(auto& filter : filters) {
			double fpr = stats[u]->getFalsePositiveRate();
			double fnr = stats[u]->getFalseNegativeRate();

			// Log("\t[Test] [Filter] ", filter->name());
			// Log("\t\t [Filter statistics] FPR: ", fpr, " FNR: ", fnr);
			// Log("\t\t [Filter statistics] Error: ", fpr + fnr);

			size_t m = u / num_filters;
			size_t f = u % num_filters;

			// Fill in result table
			results[s][m][f][0] = fpr * 100;
			results[s][m][f][1] = fnr * 100;

			++u;
		}
                
		// Delete stream after use
		Log("\t[Test] [/Stream] End of stream (", stream->name(), ")");
		streams.pop_back();
		++s;
	}
	
	auto t3 = std::chrono::high_resolution_clock::now();

	// End test
	// --------
	
	Log("\t[/Test] Tests finished.");
	
	for(size_t st = 0; st < num_streams; ++st) {
		Log("\t [Results] Stream #", st, " (", stream_duplicates[st], "% duplicates)");

		std::cout << "Mem\\fil\t |";
		size_t c = 0;
		for(auto& filter : filters) {
			if (c < num_filters) {
				std::cout << filter->name() << "\t |";
				++c;
			}
			else {
				break;
			}
		}
		std::cout << std::endl;

		for(size_t m = 0; m < num_multipliers; ++m) {
			std::cout << memory * memory_multipliers[m] << "\t |";

			for(size_t f = 0; f < num_filters; ++f) {
				std::cout << /*round_dec(results[st][m][f][0]) 
					  << "/" 
					  << round_dec(results[st][m][f][1]) 
					  << " (" 
					  <<*/ round_dec(results[st][m][f][0] + results[st][m][f][1]) 
					  << /*")*/" \t | ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}


        std::cout << std::endl << std::endl << "=============================" << std::endl << std::endl;


        for(size_t st = 0; st < num_streams; ++st) {
		Log("\t [Results] Stream #", st, " (", stream_duplicates[st], "% duplicates)");

		std::cout << "Mem\\fil\t |";
		size_t c = 0;
		for(auto& filter : filters) {
			if (c < num_filters) {
				std::cout << filter->name() << "\t |";
				++c;
			}
			else {
				break;
			}
		}
		std::cout << std::endl;

		for(size_t m = 0; m < num_multipliers; ++m) {
			std::cout << memory * memory_multipliers[m] << "\t |";

			for(size_t f = 0; f < num_filters; ++f) {
				std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(time_insertion[st][m][f]).count() / (double) stream_length
					  << " \t\t | ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}






	Log("\t[Timings]");

	auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0);
	Log("\t\t[Timings] Streams creation:\t", delta.count(), " ns");

	delta = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);
	Log("\t\t[Timings] Filters creation:\t", delta.count(), " ns");
	
	delta = std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2);
	Log("\t\t[Timings] Test duration:   \t",    delta.count(), " ns");
	
	delta = std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t0);
	Log("\t\t[Timings] Total duration:  \t",   delta.count(), " ns");

}
