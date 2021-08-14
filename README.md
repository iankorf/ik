The ik Library
==============

Functions for building simple sequence analysis programs. The philosophy behind
this library is to keep things simple and native. For example, nucleotide and
protein sequences do not have their own data types: they are just strings.

## Core Functions ##

+ Toolbox
	+ Dynamic arrays
	+ Maps (diconary, hash)
	+ Error output
	+ Command line parsing
+ Sequence
	+ FASTA files
	+ Utility functions
		+ revcomp
		+ translate
+ Model
	+ Position weight matrices
	+ Markov models (more like kmers)
	+ Length models (defined region, geometric tail)
+ Feature
	+ GFF files
	+ GFF features
	+ Simple features
	+ mRNAs
	+ Utilities
		+ revcomp everything

## Demo Programs ##

There are a few programs included to demonstrate how to use the library.

+ dusty - complexity filter
+ smithy - pairwise alignment
+ geney - sequence features

## To Do ##

+ all code needs _modernization_ checks
+ ik-test needs more tests
	+ mRNA
	+ utilities
+ smithy needs work
+ translate function
+ translate mRNA
