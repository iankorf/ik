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

void set_matrix (int);

struct ik_HSP {
	int    length;
	int    score;
	char * a1;
	char * a2;
	char * a3;
};
typedef struct ik_HSP * ik_hsp;
void ik_hsp_free (ik_hsp);
ik_hsp ik_hsp_new (void);
ik_hsp ik_sw_mmg (const char *, const char *, int, int, int);
int    ik_sw_mmglm (const char *, const char *, int, int, int);
ik_hsp ik_sw_mat (const char *, const char *, int);
int    ik_sw_matlm (const char *, const char *, int);

#endif
