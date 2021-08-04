/*****************************************************************************\
 dusty.c
\*****************************************************************************/

#include <stdio.h>

#include "toolbox.h"
#include "sequence.h"

static char *usage = "\
dusty - a low complexity filter for demonstration purposes\n\n\
usage: dusty <file> [options]\n\
options:\n\
  -w <int>    window size [11]\n\
  -h <float>  entropy threshold [1.1]\n\
  -n          use Ns for masking (default lowercase)\n\
  -a <int>    algorithm [1]";

static double entropy(int a, int c, int g, int t) {
	int total = a + c + g + t;
	double h = 0;
	double pa, pc, pg, pt;
	
	if (total == 0) return 0;
	
	pa = a / (double)total;
	pc = c / (double)total;
	pg = g / (double)total;
	pt = t / (double)total;
	
	if (a > 0) h -= pa * log(pa);
	if (c > 0) h -= pc * log(pc);
	if (g > 0) h -= pg * log(pg);
	if (t > 0) h -= pt * log(pt);
	
	return h/log(2);
}

static char *dust1(const ik_fasta ff, int w, int t, int lc) {
	int A, C, G, T;
	int pos;
	double h = 0;
	char *mask = malloc(ff->length+1);
	strcpy(mask, ff->seq);
	for (int i = 0; i < ff->length; i++) mask[i] = toupper(mask[i]);
	for (int i = 0; i < ff->length -w + 1; i++) {
		A = 0; C = 0; G = 0; T = 0;
		for (int j = 0; j < w; j++) {
			switch (mask[i+j]) {
				case 'A': A++; break;
				case 'C': C++; break;
				case 'G': G++; break;
				case 'T': T++; break;
			}
		}
		h = entropy(A, C, G, T);
		if (h < t) {
			pos = i + w/2;
			if (lc) mask[pos] = tolower(mask[pos]);
			else    mask[pos] = 'N';
		}
	}
	return mask;
}

static char *dust2(const ik_fasta ff, int w, int t, int lc) {
	int A, C, G, T;
	int pos;
	double h;
	char *mask = malloc(ff->length+1);
	strcpy(mask, ff->seq);
	for (int i = 0; i < ff->length; i++) mask[i] = toupper(mask[i]);
	
	
	// first window
	A = 0; C = 0; G = 0; T = 0;
	for (int i = 0; i < w; i++) {
		switch (mask[i]) {
			case 'A': A++; break;
			case 'C': C++; break;
			case 'G': G++; break;
			case 'T': T++; break;
		}
	}
	
	h = entropy(A, C, G, T);
	if (h < t) {
		pos = w/2;
		if (lc) mask[pos] = tolower(mask[pos]);
		else    mask[pos] = 'N';
	}

	// remaining windows
	for (int i = 1; i < ff->length -w + 1; i++) {
		char on  = mask[i+w-1];
		char off = mask[i-1];
		
		switch (on) {
			case 'A': A++; break;
			case 'C': C++; break;
			case 'G': G++; break;
			case 'T': T++; break;
		}
		
		switch (off) {
			case 'A': A--; break;
			case 'C': C--; break;
			case 'G': G--; break;
			case 'T': T--; break;
		}

		h = entropy(A, C, G, T);
		if (h < t) {
			pos = i + w/2;
			if (lc) mask[pos] = tolower(mask[pos]);
			else    mask[pos] = 'N';
		}
	}
	
	return mask;
}

int main(int argc, char **argv) {
	char *file = NULL; // path to fasta file
	int   w = 11;      // window size
	float h = 1.1;     // entropy
	int   lc = 1;      // lowercase?
	int   alg = 1;     // algorithm
	ik_pipe io;
	ik_fasta in, out;
	char *mask = NULL;
	
	// Command Line Interface
	ik_set_program_name(argv[0]);
	ik_register_option("-i", 1);
	ik_register_option("-w", 1);
	ik_register_option("-h", 1);
	ik_register_option("-n", 0);
	ik_register_option("-a", 1);
	ik_parse_options(&argc, argv);
	
	if (argc == 1) ik_exit("%s", usage);
	
	file = argv[1];
	if (ik_option("-w")) w = atoi(ik_option("-w"));
	if (ik_option("-h")) h = atof(ik_option("-h"));
	if (ik_option("-n")) lc = 0;
	if (ik_option("-a")) alg = atoi(ik_option("-a"));
	
	// main loop
	io = ik_pipe_open(file, "r");
	while ((in = ik_fasta_read(io->stream)) != NULL) {
		switch (alg) {
			case 1: mask = dust1(in, w, h, lc); break;
			case 2: mask = dust2(in, w, h, lc); break;
			default: ik_exit("algorithm out of range");
		}	
		
		out = ik_fasta_new(in->def, mask);
		ik_fasta_write(stdout, out);
		
		ik_free(mask);
		ik_fasta_free(in);
		ik_fasta_free(out);
	}

	return 0;
}
