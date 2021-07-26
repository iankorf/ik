/******************************************************************************\
 sequence.h
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_SEQUENCE_H
#define IK_SEQUENCE_H

#include <stdio.h>
#include <string.h>
#include "toolbox.h"

// fasta file
struct ik_FASTA {
	int    length;
	char * def;
	char * seq;
};
typedef struct ik_FASTA * ik_fasta;
void	 ik_fasta_free(ik_fasta);
ik_fasta ik_fasta_new(const char *, const char *);
ik_fasta ik_fasta_read(FILE *);
void	 ik_fasta_write(FILE *, const ik_fasta);
void	 ik_fasta_set_line_length(int);

// utilities
char * ik_revcomp(const char*);

#endif
