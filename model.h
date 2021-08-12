/******************************************************************************\
 model.h
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_MODEL_H
#define IK_MODEL_H

#include "toolbox.h"

// Utilities

int dna2dec(const char *, int, int);
double prob2score(double);

// Position Weight Matrix

struct ik_PWM {
	char    *name;   // acceptor, donor
	int      size;   // eg. 6
	double **score;  // score[pos][nt]
};
typedef struct ik_PWM * ik_pwm;
ik_pwm ik_pwm_read(const char *);
double ik_pwm_score(const ik_pwm, const char *, int);

// Markov model

struct ik_MM {
	char   *name;   // exon, intron
	int     k;      // kmer size
	int     size;   // size of array
	double *score;  // score[dna2dec()] = value
};
typedef struct ik_MM * ik_mm;
ik_mm  ik_mm_read(const char *);
double ik_mm_score(const ik_mm, const char *, int, int);
double * ik_mm_cache(const ik_mm, const char *);

// Length model

struct ik_LEN {
	char   *name;   // exon, intron
	int     size;   // defined region
	double *score;  // the values for defined region
	double  tail;   // mean of geometric tail
};
typedef struct ik_LEN * ik_len;
ik_len ik_len_read(const char *);
double ik_len_score(const ik_len, int);

#endif
