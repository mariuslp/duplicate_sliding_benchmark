This is the code that was used for the benchmark in the paper "Quotient Hast Tables - Efficiently Detecting Duplicates in Streaming Data", to be published in SIGAPP SAC '19.

## Datasets

It uses both artifial datasets, generated on the spot, and a real dataset, generated from CommonCrawl public data.

The real dataset, being of significant size (2.5 GB), is not included in this repository.

It can be obtained by [download](https://drive.google.com/file/d/1p52hPMl-HTpVZHwhRU54jy5OEFvFC3Sh/view?usp=sharing), or by generating it again.  
For this, execute `install_data.sh`. You will need Python with xxhash (`pip install xxhash`) and parallel (`sudo apt-get install parallel`).

The install script will download quite a lot of data (~10 GB) from CommonCrawl public dataset before unzipping it (~50 GB) and then processing it. At the end of the install only the 2.5 GB file (152,718,985 64-bits hashes) will remain.

## Requirements

The code uses boost (`sudo apt-get install libboost-dev`). Compilation is done via the Makefile (type `make`), generated binary is `build/qht`. The compiler must be C++17 compliant.

The benchmark, when running on 150,000,000 elements, will require a decent amount of RAM (~5-6 GB). The program will likely fail if there is too little memory.

## Notes

* Hash functions return `size_t` hashes, which are 64 bits long (or 32 on a 32-bits machine). This code may underperform on 32-bits machine.
* Other filters than QHT and its variants have not been space optimized. Due to specific implementation the parameter choice may be a bit restrained.

