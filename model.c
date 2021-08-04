/******************************************************************************\
 model.c
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_MODEL_C
#define IK_MODEL_C

#include "model.h"

// utilities

int dna2dec(const char *kmer) {
	int k = strlen(kmer);
	int idx = 0;
	for (int i = 0; i < k; i++) {
		switch (kmer[i]) {
			case 'A': case 'a': idx += pow(4, (k -i -1)) * 0; break;
			case 'C': case 'c': idx += pow(4, (k -i -1)) * 1; break;
			case 'G': case 'g': idx += pow(4, (k -i -1)) * 2; break;
			case 'T': case 't': idx += pow(4, (k -i -1)) * 3; break;
			default: return -1;
		}
	}
	return idx;
}

double prob2score(double p) {
	if (p == 0) return -100;
	return log(p) / log(2);
}

// PWM

ik_pwm ik_read_pwm(const char *filename) {
	char    *line = NULL;
	size_t   len = 0;
	ssize_t  read;
	ik_pipe  io  = ik_pipe_open(filename, "r");
	char     blah[256];
	int      size;
	double **score = NULL;
	double   a, c, g, t;
	int      row = 0;
	
	while ((read = getline(&line, &len, io->stream)) != -1) {
		if (line[0] == '#') {
			assert(sscanf(line, "# PWM %s %d", blah, &size) == 2);
			score = malloc(sizeof(double*) * size);
			for (int i = 0; i < size; i++) {
				score[i] = malloc(sizeof(double) * 4);
			}
		} else if (sscanf(line, "%lf %lf %lf %lf", &a, &c, &g, &t) == 4) {
			score[row][0] = prob2score(a);
			score[row][1] = prob2score(c);
			score[row][2] = prob2score(g);
			score[row][3] = prob2score(t);
			row++;
		}
	}
	ik_pipe_close(io);
	if (line) free(line);
	
	ik_pwm model = malloc(sizeof(struct ik_PWM));
	model->name = malloc(strlen(filename)+1);
	strcpy(model->name, filename);
	model->size = size;
	model->score = score;
	
	return model;
}

double ik_score_pwm(const ik_pwm pwm, const char *seq, int pos) {
	double p = 0;
	for (int i = 0; i < pwm->size; i++) {
		switch (seq[i+pos]) {
			case 'A': case 'a': p += pwm->score[i][0]; break;
			case 'C': case 'c': p += pwm->score[i][1]; break;
			case 'G': case 'g': p += pwm->score[i][2]; break;
			case 'T': case 't': p += pwm->score[i][3]; break;
			default: p += -2; // 0.25 or should I throw error?
		}
	}
	return p;
}

// Markov model

ik_mm ik_read_mm(const char *filename) {
	char    *line = NULL;
	size_t   len = 0;
	ssize_t  read;
	ik_pipe  io  = ik_pipe_open(filename, "r");
	double  *score = NULL;
	char     kmer[16];
	char     blah[256];
	int      size;
	double   p;
	
	while ((read = getline(&line, &len, io->stream)) != -1) {
		if (line[0] == '#') {
			assert(sscanf(line, "# MM %s %d", blah, &size) == 2);
			score = malloc(sizeof(double) * size);
		} else if (sscanf(line, "%s %lf", kmer, &p) == 2) {
			int idx = dna2dec(kmer);
			if (idx == -1) ik_exit("alphabet error in: %s", kmer);
			score[idx] = prob2score(p);
		}
	}
	ik_pipe_close(io);
	if (line) free(line);
	
	ik_mm model = malloc(sizeof(struct ik_MM));
	model->name = malloc(strlen(filename)+1);
	strcpy(model->name, filename);
	model->k = strlen(kmer);
	model->size = size;
	model->score = score;
	
	return model;
}

double ik_score_mm(const ik_mm mm, const char *seq, int pos, int end) {
	char kmer[16];
	double p = 0;
	
	if (pos < mm->k) pos = mm->k;
	for (int i = pos; i <= end; i++) {
		strncpy(kmer, seq+i, mm->k);
		kmer[mm->k] = '\0';
		int idx = dna2dec(kmer);
		if (idx == -1) p += -2; // 0.25 or error?
		else           p += mm->score[idx];
	}
	
	return p;
}

// Length model

static double find_tail(double val, int x) {
	double lo = 0;
	double hi = 1000;
	double m;
	
	while (hi - lo > 1) {
		m = (hi + lo) / 2;
		double p = 1 / m;
		double f = pow(1-p, x-1) * p;
		//printf("%f %f %f %f\n", lo, hi, m, f);
		if (f < val) lo += (m - lo) / 2;
		else         hi -= (hi - m) / 2;
	}

	return m;
}

ik_len ik_read_len(const char *filename) {
	char   *line = NULL;
	size_t  len = 0;
	ssize_t read;
	ik_pipe io = ik_pipe_open(filename, "r");
	double *score;
	double  p;
	int     idx = 0;
	int     size;
	double  last;
	char    blah[64];
	
	while ((read = getline(&line, &len, io->stream)) != -1) {
		if (line[0] == '#') {
			assert(sscanf(line, "# LEN %s %d", blah, &size) == 2);
			score = malloc(sizeof(double) * size);
		} else if (sscanf(line, "%lf", &p) == 1) {
			last = p;
			score[idx] = prob2score(p);
			idx++;
		}
	}
	
	ik_pipe_close(io);
	if (line) free(line);
	
	ik_len model = malloc(sizeof(struct ik_LEN));
	model->name = malloc(strlen(filename)+1);
	strcpy(model->name, filename);
	model->score = score;
	model->size = size;
	model->tail = find_tail(last, size);
	
	return model;
}

double ik_score_len(const ik_len len, int x) {
	assert(x >= 0);
	if (x >= len->size) {
		double p = 1 / len->tail;
		return pow(1-p, x-1) * p;
	} else {
		return len->score[x];
	}
}


#endif
