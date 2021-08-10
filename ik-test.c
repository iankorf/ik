#include <stdio.h>
#include <assert.h>
#include "toolbox.h"
#include "sequence.h"
#include "model.h"
#include "feature.h"

static int ITERATIONS = 9999;
void test_ivec(void);
void test_fvec(void);
void test_tvec(void);
void test_vec(void);
void test_tmap(void);
void test_map(void);
void test_feat(void);
void test_mRNA(void);

static char usage[] = "\
usage: ik-test [options]\n\
options:\n\
  -all\n\
  -ivec\n\
  -fvec\n\
  -tvec\n\
  -tmap\n\
  -map\n\
  -feat\n\
  -mRNA\n]\
  -iterate <int> [9999]\n\
";

int main(int argc, char ** argv) {

	/* preamble */
	if (argc == 1) {
		fprintf(stderr, "%s", usage);
		exit(1);
	}

	/* options */
	ik_set_program_name(argv[0]);
	ik_register_option("-iterate", 1);
	ik_register_option("-ivec", 0);
	ik_register_option("-fvec", 0);
	ik_register_option("-tvec", 0);
	ik_register_option("-vec", 0);
	ik_register_option("-tmap", 0);
	ik_register_option("-map", 0);
	ik_register_option("-feat", 0);
	ik_register_option("-mRNA", 0);
	ik_register_option("-all", 0);
	ik_parse_options(&argc, argv);

	/* control */
	if (ik_option("-iterate")) ITERATIONS = atoi(ik_option("-iterate"));
	if (ik_option("-ivec")) test_ivec();
	if (ik_option("-fvec")) test_fvec();
	if (ik_option("-tvec")) test_tvec();
	if (ik_option("-tmap")) test_tmap();
	if (ik_option("-map"))	test_map();
	if (ik_option("-feat")) test_feat();
	if (ik_option("-mRNA")) test_mRNA();
	if (ik_option("-all")) {
		test_ivec();
		test_fvec();
		test_tvec();
		test_map();
		test_tmap();
		test_feat();
		test_mRNA();
	}

	return 0;
}

void test_ivec(void) {
	int i, j;
	ik_ivec vec;

	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "ivec: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		vec = ik_ivec_new();
		for (j = 0; j < 99999; j++) {
			ik_ivec_push(vec, j);
		}
		for (j = 0; j < 99999; j++) {
			if (vec->elem[j] != j) ik_exit("ivec elem failure");
		}
		for (j = 99998; j >= 0; j--) {
			int v = ik_ivec_pop(vec);
			if (v != j) ik_exit("ivec pop failure");
		}
		ik_ivec_free(vec);

	}
	fprintf(stderr, "\n");
}

void test_fvec(void) {
	int i, j;
	ik_fvec vec;

	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "fvec: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		vec = ik_fvec_new();
		for (j = 0; j < 99999; j++) {
			ik_fvec_push(vec, j);
		}
		for (j = 0; j < 99999; j++) {
			if (vec->elem[j] != j) ik_exit("ivec elem failure");
		}
		for (j = 99998; j >= 0; j--) {
			int v = ik_fvec_pop(vec);
			if (v != j) ik_exit("ivec pop failure");
		}
		ik_fvec_free(vec);

	}
	fprintf(stderr, "\n");
}

void test_tvec(void) {
	int i, j;
	ik_tvec tvec;
	char text[16];

	sprintf(text, "hello world");
	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "tvec: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		tvec = ik_tvec_new();
		for (j = 0; j < 9999; j++) {
			ik_tvec_push(tvec, text);
		}
		for (j = 0; j < 9999; j++) {
			if (strcmp(tvec->elem[j], text) != 0) ik_exit("tvec elem failure");
		}
		for (j = 9998; j >= 0; j--) {
			char  *v = ik_tvec_pop(tvec);
			if (strcmp(v, text) != 0) ik_exit("tvec pop failure");
			free(v);
		}
		ik_tvec_free(tvec);
	}
	fprintf(stderr, "\n");
}

void test_map(void) {
	int i, j;
	ik_map map;
	char text[64];

	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "map: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		map = ik_map_new();
		for (j = 0; j < 999; j++) {
			sprintf(text, "key %d", j);
			ik_map_set(map, text, text);
		}
		ik_map_free(map);
	}
	fprintf(stderr, "\n");
}

void test_tmap(void) {
	int i, j;
	ik_tmap tmap;
	char text[64];

	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "tmap: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		tmap = ik_tmap_new();
		for (j = 0; j < 555; j++) {
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
	fprintf(stderr, "\n");
}

void test_feat(void) {
	char *seq = "NNNNNAAAAAGTAAGTTTTTTTTCAGAAAAANNNNN";
	for (int i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "feat: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		for (int j = 0; j < 9999; j++) {
			ik_feat f = ik_feat_new(seq, 10, 25);
			char *s = ik_feat_seq(f);
			if (strcmp(s, "GTAAGTTTTTTTTCAG") != 0) ik_exit("feat bad");
			free(s);
			ik_feat_free(f);
		}
	}
	fprintf(stderr, "\n");
}

void test_mRNA(void) {
	char *seq = "NNNNNAAAAAGTAAGTTTTTTTTCAGAAAAANNNNN";
	ik_ivec dons = ik_ivec_new();
	ik_ivec accs = ik_ivec_new();
	ik_ivec_push(dons, 10);
	ik_ivec_push(accs, 25);
	for (int i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "mRNA: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		for (int j = 0; j < 2222; j++) {
			ik_mRNA tx = ik_mRNA_new(seq, 5, 30, dons, accs);
			for (int j = 0; j < tx->introns->size; j++) {
				ik_feat intron = tx->introns->elem[j];
				char *s = ik_feat_seq(intron);
				if (strcmp(s, "GTAAGTTTTTTTTCAG") != 0) ik_exit("bad");
				free(s);
			}
			for (int j = 0; j < tx->exons->size; j++) {
				ik_feat exon = tx->exons->elem[j];
				char *s = ik_feat_seq(exon);
				if (strcmp(s, "AAAAA") != 0) ik_exit("bad");
				free(s);
			}
			ik_mRNA_free(tx);
		}
	}
	fprintf(stderr, "\n");
}
