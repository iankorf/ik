/*****************************************************************************\
 smithy.c
\*****************************************************************************/

#include "toolbox.h"
#include "sequence.h"
#include "align.h"

void stuff (int m, int n, int g, int a) {

}

static char *usage = "\
sw - smith-waterman for demonstration purposes\n\n\
usage: sw <file1> <file2> [options]\n\
options:\n\
  -m <int>  match score [1]\n\
  -n <int>  mismatch score [-1]\n\
  -g <int>  gap score [-2]\n\
  -a <int>    algorithm [1]";

int main(int argc, char **argv) {
	char *file1 = NULL; // path to fasta file
	char *file2 = NULL; // path to fasta file
	int   m =  1;       // match score, positive
	float n = -1;       // mismatch score, negative
	int   g = -2;       // gap score, negative
	int   a =  1;       // algorithm
	ik_pipe io1, io2;
	ik_fasta ff1, ff2;
	
	// Command Line Interface
	ik_set_program_name(argv[0]);
	ik_register_option("-m", 1);
	ik_register_option("-n", 1);
	ik_register_option("-h", 1);
	ik_register_option("-a", 1);
	ik_parse_options(&argc, argv);
	
	if (argc == 1) ik_exit("%s", usage);
	
	file1 = argv[1];
	file2 = argv[2];
	if (ik_option("-m")) m = atoi(ik_option("-m"));
	if (ik_option("-n")) n = atoi(ik_option("-n"));
	if (ik_option("-g")) g = atoi(ik_option("-g"));
	if (ik_option("-a")) a = atoi(ik_option("-a"));
	
	// main loop
	io1 = ik_pipe_open(file1, "r");
	io2 = ik_pipe_open(file2, "r");
	while ((ff1 = ik_fasta_read(io1->stream)) != NULL) {
		while ((ff2 = ik_fasta_read(io2->stream)) != NULL) {
		
			stuff(m, n, g, a);
		
			ik_fasta_free(ff1);
			ik_fasta_free(ff2);
		}
	}

	return 0;
}
