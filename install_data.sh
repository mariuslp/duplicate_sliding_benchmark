#!/bin/bash

echo "Downloading data and extracting it..."
parallel --bar install/download_data.sh ::: $(seq -f "%05g" 0 10)

echo "Processing and sorting data..."
sort cut-cdx-* -snk2 | cut -f1 -d ' ' > urls_ordered.data

rm -f cut-cdx-*

echo "Hashing data..."
python install/hash_sorted.py urls_ordered.data

rm -f urls_ordered.data
