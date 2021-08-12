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
  -imm  <file> intron Markov model (requires -dpwm & -apwm)\n\
  -elen <file> exon length model\n\
  -ilen <file> intron length model";

int main(int argc, char **argv) {
	char  *ffile = NULL; // path to fasta file
	char  *gfile = NULL; // path to gff file
	char   buff[64];
	ik_pwm dpwm  = NULL;
	ik_pwm apwm  = NULL;
	ik_mm  emm   = NULL;
	ik_mm  imm   = NULL;
	ik_len ilen  = NULL;
	ik_len elen  = NULL;

	// CLI
	ik_set_program_name(argv[0]);
	ik_register_option("-dpwm", 1);
	ik_register_option("-apwm", 1);
	ik_register_option("-emm", 1);
	ik_register_option("-imm", 1);
	ik_register_option("-elen", 1);
	ik_register_option("-ilen", 1);
	ik_parse_options(&argc, argv);
	if (argc != 3) ik_exit("%s", usage);

	ffile = argv[1];
	gfile = argv[2];
	if (ik_option("-dpwm")) dpwm = ik_pwm_read(ik_option("-dpwm"));
	if (ik_option("-apwm")) apwm = ik_pwm_read(ik_option("-apwm"));
	if (ik_option("-emm"))  emm  = ik_mm_read(ik_option("-emm"));
	if (ik_option("-imm"))  imm  = ik_mm_read(ik_option("-imm"));
	if (ik_option("-elen")) elen = ik_len_read(ik_option("-elen"));
	if (ik_option("-ilen")) ilen = ik_len_read(ik_option("-ilen"));

	// construct mRNA
	ik_pipe  io = ik_pipe_open(ffile, "r");
	ik_fasta ff = ik_fasta_read(io->stream);
	ik_mRNA  tx = ik_mRNA_read(gfile, ff->seq);
	
	// test scoring functions
	double score = 0;
	
	if (dpwm) {
		for (int i = 0; i < tx->introns->size; i++) {
			ik_feat intron = tx->introns->elem[i];
			double s = ik_pwm_score(dpwm, ff->seq, intron->beg);
			score += s;
			strncpy(buff, ff->seq + intron->beg, dpwm->size);
			buff[dpwm->size] = '\0';
			printf("donor: %f %s\n", s, buff);
		}
	}
	
	if (apwm) {
		for (int i = 0; i < tx->introns->size; i++) {
			ik_feat intron = tx->introns->elem[i];
			double s = ik_pwm_score(apwm, ff->seq, intron->end - apwm->size+1);
			score += s;
			strncpy(buff, ff->seq + intron->end - apwm->size +1, apwm->size);
			buff[apwm->size] = '\0';
			printf("acceptor: %f %s\n", s, buff);
		}
	}
	
	if (emm) {
		for (int i = 0; i < tx->exons->size; i++) {
			ik_feat exon = tx->exons->elem[i];
			double s = ik_mm_score(emm, ff->seq, exon->beg, exon->end);
			score += s;
			char *seq = ik_feat_seq(exon);
			printf("exon %f %s\n", s, seq);
			free(seq);
		}
	}
	
	if (imm) {
		for (int i = 0; i < tx->introns->size; i++) {
			ik_feat intron = tx->introns->elem[i];
			double s = ik_mm_score(imm, ff->seq, intron->beg + dpwm->size,
				intron->end - apwm->size);
			score += s;
			char *seq = ik_feat_seq(intron);
			printf("intron %f %s\n", s, seq);
			free(seq);
		}
	}
	
	if (elen) {
		for (int i = 0; i < tx->exons->size; i++) {
			ik_feat exon = tx->exons->elem[i];
			int len = exon->end - exon->beg + 1;
			double s = ik_len_score(elen, len);
			score += s;
			printf("exon %f %d\n", s, len);
		}
	}
	
	if (ilen) {
		for (int i = 0; i < tx->introns->size; i++) {
			ik_feat intron = tx->introns->elem[i];
			int len = intron->end - intron->beg + 1;
			double s = ik_len_score(ilen, len);
			score += s;
			printf("intron %f %d\n", s, len);
		}
	}
	
	printf("total score: %f\n", score);
	
	// clean up
	ik_fasta_free(ff);
	ik_pipe_close(io);

	return 0;
}
