/******************************************************************************\
 align.h
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_ALIGN_H
#define IK_ALIGN_H

#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toolbox.h"

// scoring matrices

struct ik_SMAT {
	char name[32];
	int  gap;
	int  score[128][128];
};
typedef struct ik_SMAT * ik_smat;
void ik_smat_free(ik_smat);
ik_smat ik_smat_blosum(int);
ik_smat ik_smat_mng(int, int, int);

// high scoring pairs

struct ik_HSP {
	int   beg1;
	int   end1;
	int   beg2;
	int   end2;
	int   length;
	int   score;
	char *s1;
	char *s2;
	char *as;
};
typedef struct ik_HSP * ik_hsp;
void ik_hsp_free (ik_hsp);
ik_hsp ik_hsp_new (void);

// alignment algorithms

ik_hsp ik_sw(const char *, const char *, ik_smat);

/*
ik_hsp ik_sw_mmg (const char *, const char *, int, int, int);
int    ik_sw_mmglm (const char *, const char *, int, int, int);
ik_hsp ik_sw_mat (const char *, const char *, int);
int    ik_sw_matlm (const char *, const char *, int);
*/

#endif
