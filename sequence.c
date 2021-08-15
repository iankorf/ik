/******************************************************************************\
 sequence.c
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_SEQUENCE_C
#define IK_SEQUENCE_C

#include "sequence.h"

// Utilities

char * ik_revcomp (const char *seq) {
	int length = strlen(seq);
	char *str = ik_malloc(length +1);
	str[strlen(seq)] = '\0';

	for (int i = 1; i <= length; i++) {
		switch (seq[i-1]) {
			case 'A': str[length -i] = 'T'; break;
			case 'a': str[length -i] = 't'; break;
			case 'C': str[length -i] = 'G'; break;
			case 'c': str[length -i] = 'g'; break;
			case 'G': str[length -i] = 'C'; break;
			case 'g': str[length -i] = 'c'; break;
			case 'T': str[length -i] = 'A'; break;
			case 't': str[length -i] = 'a'; break;
			case 'N': str[length -i] = 'N'; break;
			case 'n': str[length -i] = 'n'; break;
			case 'R': str[length -i] = 'Y'; break;
			case 'r': str[length -i] = 'y'; break;
			case 'Y': str[length -i] = 'R'; break;
			case 'y': str[length -i] = 'r'; break;
			case 'W': str[length -i] = 'S'; break;
			case 'w': str[length -i] = 's'; break;
			case 'S': str[length -i] = 'W'; break;
			case 's': str[length -i] = 'w'; break;
			case 'K': str[length -i] = 'M'; break;
			case 'k': str[length -i] = 'm'; break;
			case 'M': str[length -i] = 'K'; break;
			case 'm': str[length -i] = 'k'; break;
			case 'B': str[length -i] = 'V'; break;
			case 'b': str[length -i] = 'v'; break;
			case 'D': str[length -i] = 'H'; break;
			case 'd': str[length -i] = 'h'; break;
			case 'H': str[length -i] = 'D'; break;
			case 'h': str[length -i] = 'd'; break;
			case 'V': str[length -i] = 'B'; break;
			case 'v': str[length -i] = 'b'; break;
			default:  ik_exit("alphabet error %c", seq[i-1]);
		}
	}

	return str;
}

// FASTA file

void ik_fasta_free(ik_fasta ff) {
	free(ff->def);
	free(ff->seq);
	free(ff);
}

ik_fasta ik_fasta_new (const char *def, const char *seq) {
	ik_fasta ff = ik_malloc(sizeof(struct ik_FASTA));
	ff->def	    = ik_malloc(strlen(def) +1);
	ff->seq	    = ik_malloc(strlen(seq) +1);
	ff->length  = strlen(seq);
	strcpy(ff->def, def);
	strcpy(ff->seq, seq);
	return ff;
}

ik_fasta ik_fasta_read(FILE *stream) {
	
	// check for fasta header
	char c = fgetc(stream);
	if (c == EOF) return NULL;
	if (c != '>') ik_exit("ik_fasta: doesn't look like a fasta file");
	ungetc(c, stream);

	// def
	size_t size = 256;
	int i = 0;
	char *def = ik_malloc(size * sizeof(char));
	while ((c = fgetc(stream)) != EOF) {
		if (c == '\n') break;
		def[i] = c;
		i++;
		if (i == size) {
			size *= 2;
			def = ik_realloc(def, size);
		}
	}
	def[i] = '\0';

	// seq
	size = 65536;
	i = 0;
	char *seq = ik_malloc(size * sizeof(char));

	while ((c = fgetc(stream)) != EOF) {
		if (c == '>') {
			ungetc(c, stream);
			break;
		}
		if (isspace((int)c)) continue;
		seq[i] = c;
		i++;
		if (i == size) {
			size *= 2;
			seq = ik_realloc(seq, size);
		}
	}
	seq[i] = '\0';

	ik_fasta ff = ik_fasta_new(def+1, seq);
	free(def);
	free(seq);
	return ff;
}

static int FASTA_LINE_LENGTH = 50;

void ik_fasta_write(FILE *stream, const ik_fasta ff) {
	if (ff->def[0] != '>') fprintf(stream, ">");
	fprintf(stream, "%s", ff->def);
	if (ff->def[strlen(ff->def) -1] != '\n') fprintf(stream, "\n");

	for (int i = 0; i < ff->length; i++) {
		fputc(ff->seq[i], stream);
		if ((i+1) % FASTA_LINE_LENGTH == 0) fprintf(stream, "\n");
	}

	fprintf(stream, "\n");
}

void ik_fasta_set_line_length (int length) {
	FASTA_LINE_LENGTH = length;
}

#endif
