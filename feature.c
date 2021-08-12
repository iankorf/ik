/******************************************************************************\
 feature.c
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_FEATURE_C
#define IK_FEATURE_C

#include <limits.h>
#include "feature.h"

// features

ik_feat ik_feat_new(const char *seq, int beg, int end) {
	assert(beg <= end);
	ik_feat f = malloc(sizeof(struct ik_FEAT));
	f->seq    = seq;
	f->beg    = beg;
	f->end    = end;
	f->score  = 0;
	return f;
}

void ik_feat_free(ik_feat f) {
	free(f);
}

char * ik_feat_seq(const ik_feat f) {
	int len = f->end - f->beg + 1;
	char *ret = ik_malloc(len + 1);
	strncpy(ret, f->seq+f->beg, len);
	ret[len] = '\0';
	return ret;
}

// mRNA

ik_mRNA ik_mRNA_new(const char *seq, int beg, int end,
		const ik_ivec dons, const ik_ivec accs) {
	assert(beg <= end);
	ik_mRNA tx = malloc(sizeof(struct ik_MRNA));
	tx->seq     = seq;
	tx->beg     = beg;
	tx->end     = end;
	tx->exons   = ik_vec_new();
	tx->introns = ik_vec_new();
	tx->atg     = -1;
	tx->score   = 0;

	assert(dons->size == accs->size);

	if (dons->size == 0) {
		ik_vec_push(tx->exons, ik_feat_new(seq, beg, end));
		return tx;
	}

	// introns
	for (int i = 0; i < dons->size; i++) {
		int beg = dons->elem[i];
		int end = accs->elem[i];
		ik_feat f = ik_feat_new(seq, beg, end);
		ik_vec_push(tx->introns, f);
	}

	// exons
	ik_feat ei = ik_feat_new(seq, beg, dons->elem[0] -1);
	ik_vec_push(tx->exons, ei);
	for (int i = 1; i < dons->size; i++) {
		int a = accs->elem[i-1] +1;
		int b = dons->elem[i] -1;
		ik_feat ex = ik_feat_new(seq, a, b);
		ik_vec_push(tx->exons, ex);
	}
	ik_feat et = ik_feat_new(seq, accs->elem[accs->size -1] + 1, end);
	ik_vec_push(tx->exons, et);

	return tx;
}

ik_mRNA ik_mRNA_read(const char *filename, const char *seq) {
	char    *line = NULL;
	size_t   len = 0;
	ssize_t  read;
	ik_pipe  io  = ik_pipe_open(filename, "r");
	char sid[32];
	char src[32];
	char typ[32];
	int  beg;
	int  end;
	char sco[16];
	char str[8];
	char pha[8];
	char grp[1024];
	
	ik_mRNA tx = malloc(sizeof(struct ik_MRNA));
	tx->seq     = seq;
	tx->beg     = INT_MAX;
	tx->end     = INT_MIN;
	tx->exons   = ik_vec_new();
	tx->introns = ik_vec_new();
	tx->atg     = -1;
	tx->score   = 0;

	while ((read = getline(&line, &len, io->stream)) != -1) {
		if (line[0] == '#') continue;
		if (sscanf(line, "%s %s %s %d %d %s %s %s %s", sid, src, typ,
				&beg, &end, sco, str, pha, grp) == 9) {
		} else if (sscanf(line, "%s %s %s %d %d %s %s %s", sid, src, typ,
				&beg, &end, sco, str, pha) == 8) {
		} else {
			ik_exit("not parsed: %s", line);
		}
		
		if (strcmp("exon", typ) != 0) continue;
		if (beg < tx->beg) tx->beg = beg;
		if (end > tx->end) tx->end = end;
		ik_feat f = ik_feat_new(seq, beg -1, end -1);
		ik_vec_push(tx->exons, f);
	}
	ik_pipe_close(io);
	if (line) free(line);
	
	for (int i = 1; i < tx->exons->size; i++) {
		ik_feat prev = tx->exons->elem[i-1];
		ik_feat this = tx->exons->elem[i];
		int ib = prev->end + 1;
		int ie = this->beg - 1;
		ik_feat intron = ik_feat_new(seq, ib, ie);
		ik_vec_push(tx->introns, intron);
	}
	
	return tx;
}

void ik_mRNA_free(ik_mRNA tx) {
	for (int i = 0; i < tx->exons->size; i++)
		ik_feat_free(tx->exons->elem[i]);
	for (int i = 0; i < tx->introns->size; i++)
		ik_feat_free(tx->introns->elem[i]);
	ik_vec_free(tx->exons);
	ik_vec_free(tx->introns);
	free(tx);
}

char * ik_mRNA_seq(const ik_mRNA tx) {
	return NULL;
}

#endif
