#!/bin/bash

wget "https://commoncrawl.s3.amazonaws.com/cc-index/collections/CC-MAIN-2018-17/indexes/cdx-${1}.gz" && gunzip cdx-${1}.gz && cut -f1,2 -d ' ' cdx-${1} > cut-cdx-${1} && rm -f cdx-${1} 

#echo "Processing and sorting data..."
#sort cut-cdx-* -kn2 | cut -f1 -d ' ' > urls_ordered.data

#echo "Hashing data..."
#python hash_sorted.py urls_ordered.data

#rm -f urls_ordered.data
