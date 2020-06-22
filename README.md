This is the code that was used for the benchmark in the paper "Approaching Optimal Duplicate Detection in a Sliding Window", to be published in COCOON'2020.

Many parts of this code are re-used from a previous benchmark, available at https://bitbucket.org/team_qht/qht/src/master/

## Dataset

This benchmark only uses artifical datasets, for reasons explained in the paper. The streams are randomly generated on the spot.

## Requirements

The code uses boost (`sudo apt-get install libboost-dev`). Compilation is done via the Makefile (type `make`), generated binary is `build/qht`. The compiler must be C++17 compliant.

## Notes

* Hash functions return `size_t` hashes, which are 64 bits long (or 32 on a 32-bits machine). This code may underperform on 32-bits machine.
* Other filters than QHT and its variants have not been space optimized. Due to specific implementation the parameter choice may be a bit restrained.

