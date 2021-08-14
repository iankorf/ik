/******************************************************************************\
 feature.h
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_FEATURE_H
#define IK_FEATURE_H

#include "toolbox.h"

// gff
struct ik_GFF {
	char  *name;    // chromosome or sequence name
	char  *source;  // whatever (ik for stuff here?)
	char  *type;    // should be SO compliant but... 
	char  *group;   // possibly not used
	int    beg;     // 0-based internally
	int    end;     // 0-based internally
	double score;   // . or double
	char   strand;  // {.+-}
	char   phase;   // {.012}
};
typedef struct ik_GFF * ik_gff;
ik_vec ik_gff_read(const char *);
ik_gff ik_gff_new(void);
void   ik_gff_free(ik_gff);

// features
struct ik_FEAT {
	const char *seq;    // parent sequence
	int         beg;    // 0-based
	int         end;    // 0-based
	double      score;  // defaults to 0, set manually
};
typedef struct ik_FEAT * ik_feat;
ik_feat ik_feat_new(const char *, int, int);
void    ik_feat_free(ik_feat);
char  * ik_feat_seq(const ik_feat);

// mRNA
struct ik_MRNA {
	const char *seq;     // parent sequence
	int         beg;     // 0-based, start of 5'UTR
	int         end;     // 0-based, end at poly-A tail
	ik_vec      exons;   // ik_feat
	ik_vec      introns; // ik_feat
	int         atg;     // CDS start, manually set, default -1
	double      score;   // defaults to 0, set manually
};
typedef struct ik_MRNA * ik_mRNA;
ik_mRNA ik_mRNA_new(const char *, int, int);
ik_mRNA ik_mRNA_read(const char *, const char *);
void    ik_mRNA_free(ik_mRNA);

#endif
