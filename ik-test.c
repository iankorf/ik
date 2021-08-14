#include <stdio.h>
#include <assert.h>
#include "toolbox.h"
#include "sequence.h"
#include "model.h"
#include "feature.h"

static int ITERATIONS = 100;
void test_vec();
void test_ivec(int);
void test_fvec(int);
void test_tvec(int);
void test_vec(int);
void test_tmap(int);
void test_map(int);
void test_feat(int);
void test_mRNA(int);

static char usage[] = "\
usage: ik-test [options]\n\
options:\n\
  -all\n\
  -vec\n\
  -ivec\n\
  -fvec\n\
  -tvec\n\
  -tmap\n\
  -map\n\
  -feat\n\
  -mRNA\n]\
  -iterate <int> [100]\n\
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
	ik_register_option("-vec", 0);
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
	int update = (int)((double)ITERATIONS/50);
	if (update < 2) update = 2;
	
	if (ik_option("-vec"))  test_vec(update);
	if (ik_option("-ivec")) test_ivec(update);
	if (ik_option("-fvec")) test_fvec(update);
	if (ik_option("-tvec")) test_tvec(update);
	if (ik_option("-tmap")) test_tmap(update);
	if (ik_option("-map"))	test_map(update);
	if (ik_option("-feat")) test_feat(update);
	if (ik_option("-mRNA")) test_mRNA(update);
	if (ik_option("-all")) {
		test_vec(update);
		test_ivec(update);
		test_fvec(update);
		test_tvec(update);
		test_map(update);
		test_tmap(update);
		test_feat(update);
		test_mRNA(update);
	}

	return 0;
}

void test_vec(int update) {
	printf("vec ");
	for (int i = 0; i < ITERATIONS; i++) {
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
	for (int i = 0; i < ITERATIONS; i++) {
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

	for (int i = 0; i < ITERATIONS; i++) {
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
	for (int i = 0; i < ITERATIONS; i++) {
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
	for (int i = 0; i < ITERATIONS; i++) {
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
	for (int i = 0; i < ITERATIONS; i++) {
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
	for (int i = 0; i < ITERATIONS; i++) {
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

void test_mRNA(int update) {
/*
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
*/
}
