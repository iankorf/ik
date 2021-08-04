#include <stdio.h>
#include <assert.h>
#include "toolbox.h"
#include "sequence.h"

/*

	ik_imap and ik_fmap are buggy and have been removed until fixed

*/

static int ITERATIONS = 9999;
void test_ivec (void);
void test_fvec (void);
void test_tvec (void);
void test_vec (void);
//void test_imap (void);
//void test_fmap (void);
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
  -tmap\n\
  -map\n\
  -iterate <int> [9999]\n\
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
//	ik_register_option("-imap", 0);
//	ik_register_option("-fmap", 0);
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
//	if (ik_option("-imap")) test_imap();
//	if (ik_option("-fmap")) test_fmap();
	if (ik_option("-tmap")) test_tmap();
	if (ik_option("-map"))	test_map();
	if (ik_option("-all")) {
		test_vec();
		test_ivec();
		test_fvec();
		test_tvec();
		//test_imap();
		//test_fmap();
		test_map();
		test_tmap();
	}

	return 0;
}

void test_ivec (void) {
	int i, j;
	ik_ivec ivec;
	
	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "ivec: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		ivec = ik_ivec_new();
		for (j = 0; j < 99999; j++) {
			ik_ivec_push(ivec, j);
		}
		ik_ivec_free(ivec);
	}
	fprintf(stderr, "\n");
}

void test_fvec (void) {
	int i, j;
	ik_fvec fvec;
	
	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "fvec: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		fvec = ik_fvec_new();
		for (j = 0; j < 99999; j++) {
			ik_fvec_push(fvec, j);
		}
		ik_fvec_free(fvec);
	}
	fprintf(stderr, "\n");
}

void test_vec (void) {
	int i, j;
	ik_vec vec;
	char text[64];
	
	sprintf(text, "hello world");
	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "vec: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		vec = ik_vec_new();
		for (j = 0; j < 99999; j++) {
			ik_vec_push(vec, text);
		}
		ik_vec_free(vec);
	}
	fprintf(stderr, "\n");
}

void test_tvec (void) {
	int i, j;
	ik_tvec tvec;
	char text[64];

	sprintf(text, "hello world");
	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "tvec: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		tvec = ik_tvec_new();
		for (j = 0; j < 2000; j++) {
			ik_tvec_push(tvec, text);
		}
		ik_tvec_free(tvec);
	}
	fprintf(stderr, "\n");
}



/*
void test_imap (void) {
	int i, j;
	ik_imap imap;
	char text[64];
	
	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "imap: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		imap = ik_imap_new();
		for (j = 0; j < 400; j++) {
			sprintf(text, "key %d", j);
			ik_imap_set(imap, text, j);
		}
		ik_imap_free(imap);
	}
	fprintf(stderr, "\n");
}

void test_fmap (void) {
	int i, j;
	ik_fmap fmap;
	char text[64];
	
	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "fmap: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		fmap = ik_fmap_new();
		for (j = 0; j < 400; j++) {
			sprintf(text, "key %d", j);
			ik_fmap_set(fmap, text, j);
		}
		ik_fmap_free(fmap);
	}
	fprintf(stderr, "\n");
}
*/

void test_tmap (void) {
	int i, j;
	ik_tmap tmap;
	char text[64];
	
	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "tmap: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		tmap = ik_tmap_new();
		for (j = 0; j < 400; j++) {
			sprintf(text, "key %d", j);
			ik_tmap_set(tmap, text, text);
		}
		ik_tmap_free(tmap);
	}
	fprintf(stderr, "\n");
}

void test_map (void) {
	int i, j;
	ik_map map;
	char text[64];
	
	for (i = 0; i < ITERATIONS; i++) {
		if (i % 100 == 0) fprintf(stderr, "map: %d\r",
			(int) (100 * (float)i/ITERATIONS));
		map = ik_map_new();
		for (j = 0; j < 400; j++) {
			sprintf(text, "key %d", j);
			ik_map_set(map, text, text);
		}
		ik_map_free(map);
	}
	fprintf(stderr, "\n");
}
