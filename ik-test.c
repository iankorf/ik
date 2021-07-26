#include <stdio.h>
#include <assert.h>
#include "toolbox.h"
#include "sequence.h"

static int ITERATIONS = 1000;
void test_ivec (void);
void test_fvec (void);
void test_tvec (void);
void test_vec (void);
void test_imap (void);
void test_fmap (void);
void test_tmap (void);
void test_map (void);

static char usage[] = "\
usage: ik-test [options]\n\
options:\n\
  -all\n\
  -ivec\n\
  -fvec\n\
  -tvec\n\
  -vec\n\
  -imap\n\
  -fmap\n\
  -tmap\n\
  -map\n\
  -iterate <int> [1000]\n\
";

int main (int argc, char ** argv) {
	
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
	ik_register_option("-imap", 0);
	ik_register_option("-fmap", 0);
	ik_register_option("-tmap", 0);
	ik_register_option("-map", 0);
	ik_register_option("-all", 0);
	ik_parse_options(&argc, argv);
	
	/* control */
	if (ik_option("-iterate")) ITERATIONS = atoi(ik_option("-iterate"));
	if (ik_option("-ivec")) test_ivec();
	if (ik_option("-fvec")) test_fvec();
	if (ik_option("-tvec")) test_tvec();
	if (ik_option("-vec"))	test_vec();
	if (ik_option("-imap")) test_imap();
	if (ik_option("-fmap")) test_fmap();
	if (ik_option("-tmap")) test_tmap();
	if (ik_option("-map"))	test_map();
	if (ik_option("-all")) {
		test_ivec();
		test_fvec();
		test_tvec();
		test_imap();
		test_fmap();
		test_tmap();
		test_map();
	}

	return 0;
}

#define ILIMIT 10000

void test_ivec (void) {
	int i, j;
	ik_ivec ivec;
	
	printf("ivec\n");
	for (i = 0; i < ITERATIONS; i++) {
		ivec = ik_ivec_new();
		for (j = 0; j < 4000; j++) {
			ik_ivec_push(ivec, j);
		}
		ik_ivec_free(ivec);
	}
}

void test_fvec (void) {
	int i, j;
	ik_fvec fvec;
	
	printf("fvec\n");
	for (i = 0; i < ITERATIONS; i++) {
		fvec = ik_fvec_new();
		for (j = 0; j < 3000; j++) {
			ik_fvec_push(fvec, j);
		}
		ik_fvec_free(fvec);
	}
}

void test_tvec (void) {
	int i, j;
	ik_tvec tvec;
	char text[64];
	
	printf("tvec\n");
	for (i = 0; i < ITERATIONS; i++) {
		tvec = ik_tvec_new();
		for (j = 0; j < 200; j++) {
			sprintf(text, "string %d", j);
			ik_tvec_push(tvec, text);
		}
		ik_tvec_free(tvec);
	}
}

void test_vec (void) {
	int i, j;
	ik_vec vec;
	char text[64];
	
	printf("vec\n");
	for (i = 0; i < ITERATIONS; i++) {
		vec = ik_vec_new();
		for (j = 0; j < 200; j++) {
			sprintf(text, "string %d", j);
			ik_vec_push(vec, text);
		}
		ik_vec_free(vec);
	}
}

void test_imap (void) {
	int i, j;
	ik_imap imap;
	char text[64];
	
	printf("imap\n");
	for (i = 0; i < ITERATIONS; i++) {
		imap = ik_imap_new();
		for (j = 0; j < 40; j++) {
			sprintf(text, "key %d", j);
			ik_imap_set(imap, text, j);
		}
		ik_imap_free(imap);
	}
}

void test_fmap (void) {
	int i, j;
	ik_fmap fmap;
	char text[64];
	
	printf("fmap\n");
	for (i = 0; i < ITERATIONS; i++) {
		fmap = ik_fmap_new();
		for (j = 0; j < 40; j++) {
			sprintf(text, "key %d", j);
			ik_fmap_set(fmap, text, j);
		}
		ik_fmap_free(fmap);
	}
}

void test_tmap (void) {
	int i, j;
	ik_tmap tmap;
	char text[64];
	
	printf("tmap\n");
	for (i = 0; i < ITERATIONS; i++) {
		tmap = ik_tmap_new();
		for (j = 0; j < 40; j++) {
			sprintf(text, "key %d", j);
			ik_tmap_set(tmap, text, text);
		}
		ik_tmap_free(tmap);
	}
}

void test_map (void) {
	int i, j;
	ik_map map;
	char text[64];
	
	printf("map\n");
	for (i = 0; i < ITERATIONS; i++) {
		map = ik_map_new();
		for (j = 0; j < 40; j++) {
			sprintf(text, "key %d", j);
			ik_map_set(map, text, text);
		}
		ik_map_free(map);
	}
}
