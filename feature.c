/******************************************************************************\
 feature.c
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_FEATURE_C
#define IK_FEATURE_C

#include "feature.h"

// features

/*
	147 exon
	188 intron
	185 primary_transcript
	234 mRNA
*/

ik_feat ik_feat_new(char *seq, int beg, int end, int strand, int type) {
	ik_feat f = malloc(sizeof(struct ik_FEAT));
	f->beg    = beg;
	f->end    = end;
	f->seq    = seq;
	f->strand = strand;
	f->type   = type; // probably should use SO numbers
	return f;
}

void ik_feat_free(ik_feat f) {
	free(f);
}

char * ik_feat_seq(ik_feat f) {
	return NULL;
}

// transcripts

ik_mRNA ik_mRNA_new(ik_ivec dons, ik_ivec accs, const char *seq, int strand) {
	ik_mRNA tx = NULL;
	return tx;
}
void ik_mRNA_free(ik_mRNA tx) {
	for (int i = 0; i < tx->exons->size; i++) free(tx->exons->elem[i]);
	for (int i = 0; i < tx->introns->size; i++) free(tx->introns->elem[i]);
	ik_vec_free(tx->exons);
	ik_vec_free(tx->introns);
	free(tx);
}

char * ik_mRNA_seq(ik_mRNA tx) {
	return NULL;
}

#endif
