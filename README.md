The ik Library
==============

Functions for building simple sequence analysis programs. The philosophy behind
this library is to keep things simple and native. For example, nucleotide and
protein sequences do not have their own data types: they are just strings.

## Core Functions ##

+ Toolbox
	+ Dynamic arrays
	+ Maps (hashes)
	+ Error output
	+ Command line
+ Sequence
	+ FASTA files
	+ Utility functions
+ Model
	+ Position weight matrices
	+ Markov models (more like kmers)
	+ Length models

## Demo Programs ##

There are a few programs included to demonstrate how to use the library.

+ dusty - complexity filter
+ smithy - pairwise alignment
