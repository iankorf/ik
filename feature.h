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

struct ik_FEAT {
	int   beg;    // 0-based
	int   end;    // 0-based
	char *seq;    // parent sequence, not copied
	int   strand; // -1, 0, 1
	int   type;
};
typedef struct ik_FEAT * ik_feat;
ik_feat ik_feat_new(char *, int, int, int, int);
void    ik_feat_free(ik_feat);
char  * ik_feat_seq(ik_feat);

struct ik_MRNA {
	int    beg;
	int    end;
	char  *seq;
	int    strand;
	ik_vec exons;
	ik_vec introns;
};
typedef struct ik_MRNA * ik_mRNA;
ik_mRNA ik_mRNA_new(ik_ivec, ik_ivec, const char*, int);
void    ik_mRNA_free(ik_mRNA);
char  * ik_mRNA_seq(ik_mRNA);

#endif
