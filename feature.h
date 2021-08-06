/******************************************************************************\
 model.h
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_FEATURE_H
#define IK_FEATURE_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "toolbox.h"

// features are always positive strand at this point
struct ik_FEAT {
	const char *seq; // parent sequence, not copied
	int         beg; // 0-based
	int         end; // 0-based
};
typedef struct ik_FEAT * ik_feat;
ik_feat ik_feat_new(const char *, int, int);
void    ik_feat_free(ik_feat);
char  * ik_feat_seq(const ik_feat);

struct ik_MRNA {
	const char *seq;     // parent sequence, not copied
	int         beg;     // 0-based, start of 5'UTR
	int         end;     // 0-based, end at poly-A tail
	ik_vec      exons;   // ik_feat
	ik_vec      introns; // ik_feat
};
typedef struct ik_MRNA * ik_mRNA;
ik_mRNA ik_mRNA_new(const char*, int, int, const ik_ivec, const ik_ivec);
void    ik_mRNA_free(ik_mRNA);
char  * ik_mRNA_seq(const ik_mRNA);

#endif
