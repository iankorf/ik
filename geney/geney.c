/*****************************************************************************\
 geney.c
\*****************************************************************************/

#include <stdio.h>

#include "toolbox.h"
#include "sequence.h"
#include "feature.h"
#include "model.h"

static char *usage = "\
geney - a gene scoring demonstration program\n\n\
usage: dusty <fasta file> <gff file> [options]\n\
options:\n\
  -dpwm <file> donor model position weight matrix\n\
  -apwm <file> acceptor model position weight matrix\n\
  -emm  <file> exon Markov model\n\
  -imm  <file> intron Markov model\n\
  -elen <file> exon length model\n\
  -ilen <file> intron length model";

int main(int argc, char **argv) {
	char *ffile = NULL; // path to fasta file
	char *gfile = NULL; // path to gff file
	ik_pipe io;
	ik_fasta ff;
	ik_pwm dpwm = NULL;
	ik_pwm apwm = NULL;
	ik_mm emm = NULL;
	ik_mm imm = NULL;
	ik_len ilen = NULL;
	ik_len elen = NULL;

	// Command Line Interface
	ik_set_program_name(argv[0]);
	ik_register_option("-dpwm", 1);
	ik_register_option("-apwm", 1);
	ik_register_option("-emm", 1);
	ik_register_option("-imm", 1);
	ik_register_option("-elen", 1);
	ik_register_option("-ilen", 1);
	ik_parse_options(&argc, argv);
	if (argc == 1) ik_exit("%s", usage);

	ffile = argv[1];
	gfile = argv[2];
	if (ik_option("-dpwm")) dpwm = ik_read_pwm(ik_option("-dpwm"));
	if (ik_option("-apwm")) dpwm = ik_read_pwm(ik_option("-apwm"));
	if (ik_option("-emm"))  dpwm = ik_read_pwm(ik_option("-emm"));
	if (ik_option("-imm"))  dpwm = ik_read_pwm(ik_option("-imm"));
	if (ik_option("-elen")) dpwm = ik_read_pwm(ik_option("-elen"));
	if (ik_option("-ilen")) dpwm = ik_read_pwm(ik_option("-ilen"));

	// main loop
	io = ik_pipe_open(ffile, "r");
	ff = ik_fasta_read(io->stream);

	return 0;
}
