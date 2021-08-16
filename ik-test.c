#include <stdio.h>
#include <assert.h>
#include "toolbox.h"
#include "sequence.h"
#include "model.h"
#include "feature.h"

static int COUNT = 100;
void test_vec();
void test_ivec(int);
void test_fvec(int);
void test_tvec(int);
void test_vec(int);
void test_tmap(int);
void test_map(int);
void test_feat(int);
void test_pipe(int, const char *);
void test_fasta(int, const char *);
void test_gff(int, const char *);
void test_pwm(int, const char *);
void test_mm(int, const char *);
void test_len(int, const char*);
void test_ian(void);

static char usage[] = "\
usage: ik-test [options]\n\
options:\n\
  -vec\n\
  -ivec\n\
  -fvec\n\
  -tvec\n\
  -tmap\n\
  -map\n\
  -feat\n\
  -pipe  <file>\n\
  -fasta <file>\n\
  -gff   <file>\n\
  -pwm   <file\n\
  -mm    <file>\n\
  -len   <file\n\
  -count <int> [100]\n\
";

int main(int argc, char ** argv) {

	/* preamble */
	if (argc == 1) {
		fprintf(stderr, "%s", usage);
		exit(1);
	}

	/* options */
	ik_set_program_name(argv[0]);
	ik_register_option("-count", 1);
	ik_register_option("-vec",   0);
	ik_register_option("-ivec",  0);
	ik_register_option("-fvec",  0);
	ik_register_option("-tvec",  0);
	ik_register_option("-vec",   0);
	ik_register_option("-tmap",  0);
	ik_register_option("-map",   0);
	ik_register_option("-feat",  0);
	ik_register_option("-pipe",  1);
	ik_register_option("-fasta", 1);
	ik_register_option("-gff",   1);
	ik_register_option("-pwm",   1);
	ik_register_option("-mm",    1);
	ik_register_option("-len",   1);
	ik_register_option("-ian",   0);
	ik_parse_options(&argc, argv);

	/* control */
	if (ik_option("-count")) COUNT = atoi(ik_option("-count"));
	int update = (int)((double)COUNT/50);
	if (update < 2) update = 2;
	
	if (ik_option("-vec"))   test_vec(update);
	if (ik_option("-ivec"))  test_ivec(update);
	if (ik_option("-fvec"))  test_fvec(update);
	if (ik_option("-tvec"))  test_tvec(update);
	if (ik_option("-tmap"))  test_tmap(update);
	if (ik_option("-map"))	 test_map(update);
	if (ik_option("-feat"))  test_feat(update);
	if (ik_option("-pipe"))  test_pipe(update,  ik_option("-pipe"));
	if (ik_option("-fasta")) test_fasta(update, ik_option("-fasta"));
	if (ik_option("-gff"))   test_gff(update,   ik_option("-gff"));
	if (ik_option("-pwm"))   test_pwm(update,   ik_option("-pwm"));
	if (ik_option("-mm"))    test_mm(update,    ik_option("-mm"));
	if (ik_option("-len"))   test_len(update,   ik_option("-len"));
	if (ik_option("-ian"))   test_ian();

	return 0;
}

void test_vec(int update) {
	printf("vec ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		ik_vec vec = ik_vec_new();
		for (int j = 0; j < 99999; j++) {
			ik_vec_push(vec, NULL);
		}
		for (int j = 0; j < 99999; j++) {
			if (vec->elem[j] != NULL) ik_exit("ivec elem failure");
		}
		for (int j = 99998; j >= 0; j--) {
			void *v = ik_vec_pop(vec);
			if (v != NULL) ik_exit("ivec pop failure");
		}
		ik_vec_free(vec);

	}
	printf(" done \n");
}

void test_ivec(int update) {
	printf("ivec ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		ik_ivec vec = ik_ivec_new();
		for (int j = 0; j < 99999; j++) {
			ik_ivec_push(vec, j);
		}
		for (int j = 0; j < 99999; j++) {
			if (vec->elem[j] != j) ik_exit("ivec elem failure");
		}
		for (int j = 99998; j >= 0; j--) {
			int v = ik_ivec_pop(vec);
			if (v != j) ik_exit("ivec pop failure");
		}
		ik_ivec_free(vec);

	}
	printf(" done \n");
}

void test_fvec(int update) {
	printf("fvec ");

	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		ik_fvec vec = ik_fvec_new();
		for (int j = 0; j < 99999; j++) {
			ik_fvec_push(vec, j);
		}
		for (int j = 0; j < 99999; j++) {
			if (vec->elem[j] != j) ik_exit("ivec elem failure");
		}
		for (int j = 99998; j >= 0; j--) {
			int v = ik_fvec_pop(vec);
			if (v != j) ik_exit("ivec pop failure");
		}
		ik_fvec_free(vec);

	}
	printf(" done\n");
}

void test_tvec(int update) {
	char text[16];
	sprintf(text, "hello world");
	
	printf("tvec ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		ik_tvec tvec = ik_tvec_new();
		for (int j = 0; j < 9999; j++) {
			ik_tvec_push(tvec, text);
		}
		for (int j = 0; j < 9999; j++) {
			if (strcmp(tvec->elem[j], text) != 0) ik_exit("tvec elem failure");
		}
		for (int j = 9998; j >= 0; j--) {
			char  *v = ik_tvec_pop(tvec);
			if (strcmp(v, text) != 0) ik_exit("tvec pop failure");
			free(v);
		}
		ik_tvec_free(tvec);
	}
	printf(" done\n");
}

void test_map(int update) {
	char text[32];
	
	printf("map ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		ik_map map = ik_map_new();
		for (int j = 0; j < 999; j++) {
			sprintf(text, "key %d", j);
			ik_map_set(map, text, text);
		}
		ik_map_free(map);
	}
	printf(" done\n");
}

void test_tmap(int update) {
	char text[32];

	printf("tmap ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		ik_tmap tmap = ik_tmap_new();
		for (int j = 0; j < 555; j++) {
			sprintf(text, "key %d", j);
			ik_tmap_set(tmap, text, text);
		}

		ik_tvec keys = ik_tmap_keys(tmap);
		for (int k = 0; k < keys->size; k++) {
			char* v = ik_tmap_get(tmap, keys->elem[k]);
			if (strcmp(v, keys->elem[k]) != 0) {
				printf("key %s has value %s\n", keys->elem[k], v);
				ik_exit("tmap integrity failure");
			}
		}
		ik_tvec_free(keys);
		ik_tmap_free(tmap);
	}
	printf(" done\n");
}

void test_feat(int update) {
	char *seq = "NNNNNAAAAAGTAAGTTTTTTTTCAGAAAAANNNNN";
	printf("feat ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		for (int j = 0; j < 9999; j++) {
			ik_feat f = ik_feat_new(seq, 10, 25);
			char *s = ik_feat_seq(f);
			if (strcmp(s, "GTAAGTTTTTTTTCAG") != 0) ik_exit("feat bad");
			free(s);
			ik_feat_free(f);
		}
	}
	printf(" done\n");
}

void test_pipe(int update, const char *filename) {
	printf("pipe ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		
		for (int j = 0; j < 100; j ++) {
			ik_pipe io = ik_pipe_open(filename, "r");
			ik_pipe_close(io);
		}
	}
	printf(" done\n");
}

void test_fasta(int update, const char *filename) {
	ik_fasta in;
	printf("fasta ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		
		for (int j = 0; j < 100; j ++) {
			ik_pipe io = ik_pipe_open(filename, "r");
			while ((in = ik_fasta_read(io->stream)) != NULL) {
				ik_fasta_free(in);
			}
			ik_pipe_close(io);
		}
	}
	printf(" done\n");
}

void test_gff(int update, const char *filename) {
	ik_gff gff;
	printf("gff ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		
		for (int j = 0; j < 100; j ++) {
			ik_pipe io = ik_pipe_open(filename, "r");
			while ((gff = ik_gff_read(io->stream)) != NULL) {
				ik_gff_free(gff);
			}
			ik_pipe_close(io);
		}
	}
	printf(" done\n");
}

void test_pwm(int update, const char *filename) {
	printf("pwm ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		for (int j = 0; j < 100; j++) {
			ik_pwm pwm = ik_pwm_read(filename);
			ik_pwm_free(pwm);
		}
	}
	printf(" done\n");
}

void test_mm(int update, const char *filename) {
	printf("mm ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		for (int j = 0; j < 100; j++) {
			ik_mm mm = ik_mm_read(filename);
			ik_mm_free(mm);
		}
	}
	printf(" done\n");
}

void test_len(int update, const char *filename) {
	printf("len ");
	for (int i = 0; i < COUNT; i++) {
		if (i % update == 0) {
			printf(".");
			fflush(stdout);
		}
		for (int j = 0; j < 100; j++) {
			ik_len len = ik_len_read(filename, 1000);
			ik_len_free(len);
		}
	}
	printf(" done\n");
}

void test_ian(void) {
	ik_pipe  io  = ik_pipe_open("geney/777.fa", "r");
	ik_fasta ff  = ik_fasta_read(io->stream);
	ik_mm    mm  = ik_mm_read("geney/exon.mm");
	double  *mem = ik_mm_cache(mm, ff->seq);
	double   s0  = ik_mm_score(mm, ff->seq, 99, 178);
	double   s1  = ik_mm_score_cache(mem, 99, 178);
	printf("%f %f\n", s0, s1);
	
	for (int i = -3; i <= 3; i++) {
		for (int j = -3; j <= 3; j++) {
			int beg = 99 + i;
			int end = 178 + j;
			double s2 = mem[end] - mem[beg];
			if (fabs(s2 - s0) < 0.3) {
				printf("%d %d %f\n", i, j, s2 - s0);
			}
		}
	}
}