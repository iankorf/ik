/******************************************************************************\
 model.h
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_MODEL_H
#define IK_MODEL_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "toolbox.h"

// utilities

int dna2dec(const char *);
double prob2score(double);

// Position Weight Matrix

struct ik_PWM {
	char    *name;   // acceptor, donor
	int      size;   // eg. 6
	double **score;  // score[pos][nt]
};
typedef struct ik_PWM * ik_pwm;
ik_pwm ik_read_pwm(const char *);
double ik_score_pwm(const ik_pwm, const char *, int);

// Markov model

struct ik_MM {
	char   *name;   // exon, intron
	int     k;      // kmer size
	int     size;   // size of array
	double *score;  // score[dna2dec()] = value
};
typedef struct ik_MM * ik_mm;
ik_mm  ik_read_mm(const char *);
double ik_score_mm(const ik_mm, const char *, int, int);

// Length model

struct ik_LEN {
	char   *name;   // exon, intron
	int     size;   // defined region
	double *score;  // the values for defined region
	double  tail;   // mean of geometric tail
};
typedef struct ik_LEN * ik_len;
ik_len ik_read_len(const char *);
double ik_score_len(const ik_len, int);

#endif
