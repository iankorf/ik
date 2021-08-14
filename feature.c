/******************************************************************************\
 feature.c
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_FEATURE_C
#define IK_FEATURE_C

#include <limits.h>
#include "feature.h"

// gff

ik_gff ik_gff_read(FILE *stream) {
	char    *line = NULL;
	size_t   len = 0;
	ssize_t  read;
	char sid[32];
	char src[32];
	char typ[32];
	int  beg;
	int  end;
	char sco[16];
	char str;
	char pha;
	char grp[1024];
	
	while ((read = getline(&line, &len, stream)) != -1) {
		int groupon = 0;
		if (line[0] == '#') continue;
		if (sscanf(line, "%s %s %s %d %d %s %s %s %s", sid, src, typ,
			&beg, &end, sco, &str, &pha, grp) == 9) {
			groupon = 1;
		} else if (sscanf(line, "%s %s %s %d %d %s %s %s", sid, src, typ,
			&beg, &end, sco, &str, &pha) == 8) {
		} else {
			ik_exit("gff not parsed correctly\n %s", line);
		}
		
		ik_gff gff = ik_gff_new();
		gff->beg = beg -1;
		gff->end = end -1;
		gff->name   = malloc(strlen(sid) +1); strcpy(gff->name,   sid);
		gff->source = malloc(strlen(src) +1); strcpy(gff->source, src);
		gff->type   = malloc(strlen(typ) +1); strcpy(gff->type,   typ);
		if (groupon) {
			gff->group = malloc(strlen(grp) +1);
			strcpy(gff->group, grp);
		}
		if (strcmp(".", sco) != 0) gff->score = atof(sco);
		
		if (line) free(line);
		return gff;
	}
	
	if (line) free(line);
	return NULL;
}

ik_gff ik_gff_new(void) {
	ik_gff gff = malloc(sizeof(struct ik_GFF));
	gff->name = NULL;
	gff->source = NULL;
	gff->type = NULL;
	gff->group = NULL;
	gff->beg = INT_MIN;
	gff->end = INT_MIN;
	gff->score = -FLT_MAX;
	gff->strand = '.';
	gff->phase = '.';
	return gff;
}

void ik_gff_free(ik_gff gff) {
	if (gff->name)   free(gff->name);
	if (gff->source) free(gff->source);
	if (gff->type)   free(gff->type);
	if (gff->group)  free(gff->group);
	free(gff);
}

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

ik_mRNA ik_mRNA_new(const char *seq, int beg, int end) {
	assert(beg <= end);
	ik_mRNA tx = malloc(sizeof(struct ik_MRNA));
	tx->seq     = seq;
	tx->beg     = beg;
	tx->end     = end;
	tx->exons   = NULL;
	tx->introns = NULL;
	tx->atg     = INT_MIN;
	tx->score   = -FLT_MAX;
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
	if (tx->exons) {
		for (int i = 0; i < tx->exons->size; i++)
			ik_feat_free(tx->exons->elem[i]);
		ik_vec_free(tx->exons);
	}
	if (tx->introns) {
		for (int i = 0; i < tx->introns->size; i++)
			ik_feat_free(tx->introns->elem[i]);
	
		ik_vec_free(tx->introns);
	}
	free(tx);
}

#endif
