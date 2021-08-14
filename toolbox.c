/******************************************************************************\
 toolbox.c
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_TOOLBOX_C
#define IK_TOOLBOX_C

#include "toolbox.h"

static char ik_version_number[] = "2021";
static char ik_program_name[64] = "name not set";

char * ik_get_version_number (void) {return ik_version_number;}
void   ik_set_program_name (const char *s) {strcpy(ik_program_name, s);}
char * ik_get_program_name (void) {return ik_program_name;}

void ik_exit(const char* format, ...) {
	va_list args;

	fflush(stdout);
	fprintf(stderr, "ERROR from program %s, libarary version %s\n",
		ik_get_program_name(),
		ik_get_version_number());
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(1);
}

void * ik_malloc(size_t size) {
	void *mem;
	mem = malloc(size);
	if (mem == NULL) ik_exit("ik_malloc %d", size);
	return mem;
}

void * ik_calloc(size_t count, size_t size) {
	void *mem;
	mem = calloc(count, size);
	if (mem == NULL) ik_exit("ik_calloc %d %d", count, size);
	return mem;
}

void * ik_realloc(void *p, size_t size) {
	void *mem;
	mem = realloc(p, size);
	if (mem == NULL) ik_exit("ik_realloc %d", size);
	return mem;
}


void ik_ivec_free(ik_ivec vec) {
	if (vec == NULL) return;
	if (vec->elem) free(vec->elem);
	free(vec);
}

ik_ivec ik_ivec_new(void) {
	ik_ivec vec = ik_malloc(sizeof(struct ik_IVEC));
	vec->size  = 0;
	vec->limit = 0;
	vec->elem  = NULL;
	return vec;
}

void ik_ivec_push(ik_ivec vec, int val) {
	if (vec->limit == vec->size) {
		if (vec->limit == 0) vec->limit	 = 1;
		else				 vec->limit *= 2;
		vec->elem = ik_realloc(vec->elem, vec->limit * sizeof(int));
	}
	vec->elem[vec->size] = val;
	vec->last = val;
	vec->size++;
}

int ik_ivec_pop(ik_ivec vec) {
	if (vec->size == 0) ik_exit("can't pop a zero-length vector");
	vec->size--;
	return vec->elem[vec->size];
}

void ik_fvec_free(ik_fvec vec) {
	if (vec == NULL) return;
	if (vec->elem) free(vec->elem);
	free(vec);
}

ik_fvec ik_fvec_new(void) {
	ik_fvec vec = ik_malloc(sizeof(struct ik_FVEC));
	vec->size  = 0;
	vec->limit = 0;
	vec->elem  = NULL;
	return vec;
}

void ik_fvec_push(ik_fvec vec, float val) {
	if (vec->limit == vec->size) {
		if (vec->limit == 0) vec->limit	 = 1;
		else				 vec->limit *= 2;
		vec->elem = ik_realloc(vec->elem, vec->limit * sizeof(float));
	}
	vec->elem[vec->size] = val;
	vec->last = val;
	vec->size++;
}

float ik_fvec_pop(ik_fvec vec) {
	if (vec->size == 0) ik_exit("can't pop a zero-length vector");
	vec->size--;
	return vec->elem[vec->size];
}

void ik_tvec_free(ik_tvec vec) {
	int i;

	if (vec == NULL) return;
	if (vec->elem) {
		for (i = 0; i < vec->size; i++) free(vec->elem[i]);
		free(vec->elem);
	}
	free(vec);
}

ik_tvec ik_tvec_new(void) {
	ik_tvec vec = ik_malloc(sizeof(struct ik_TVEC));
	vec->size  = 0;
	vec->limit = 0;
	vec->elem  = NULL;
	return vec;
}

void ik_tvec_push(ik_tvec vec, const char *text) {
	if (vec->limit == vec->size) {
		if (vec->limit == 0) vec->limit	 = 1;
		else				 vec->limit *= 2;
		vec->elem = ik_realloc(vec->elem, vec->limit * sizeof(char *));
	}
	vec->elem[vec->size] = ik_malloc(strlen(text) + 1);
	strcpy(vec->elem[vec->size], text);
	vec->last = vec->elem[vec->size];
	vec->size++;
}

char * ik_tvec_pop(ik_tvec vec) {
	if (vec->size == 0) ik_exit("can't pop a zero-length vector");
	vec->size--;
	return vec->elem[vec->size];
}

void ik_vec_free(ik_vec vec) {
	if (vec == NULL) return;
	if (vec->elem) free(vec->elem);
	free(vec);
}

ik_vec ik_vec_new(void) {
	ik_vec vec = ik_malloc(sizeof(struct ik_VEC));
	vec->size  = 0;
	vec->limit = 0;
	vec->elem  = NULL;
	return vec;
}

void ik_vec_push(ik_vec vec, void *p) {
	if (vec->limit == vec->size) {
		if (vec->limit == 0) vec->limit	 = 1;
		else				 vec->limit *= 2;
		vec->elem = ik_realloc(vec->elem, vec->limit * sizeof(void *));
	}
	vec->elem[vec->size] = p;
	vec->last = vec->elem[vec->size];
	vec->size++;
}

void * ik_vec_pop(ik_vec vec) {
	if (vec->size == 0) ik_exit("can't pop a zero-length vector");
	vec->size--;
	return vec->elem[vec->size];
}

// hashing materials
static double HASH_MULTIPLIER[7] = {
	3.1415926536, // PI
	2.7182818285, // e
	1.6180339887, // golden mean
	1.7320508076, // square root of 3
	2.2360679775, // square root of 5
	2.6457513111, // square root of 7
	3.3166247904  // square root of 11
};
static float MAX_HASH_DEPTH = 2.0;
static int HashLevelToSlots(int level) {return pow(4, level);}
static int HashFunc(const ik_map hash, const char *key) {
	int	   i;
	double sum = 0;
	for (i = 0; i < strlen(key); i++) sum += key[i] * HASH_MULTIPLIER[i % 7];
	return (int) (hash->slots * (sum - floor(sum)));
}

static void ExpandHash(ik_map hash) {
	int     i, j;
	char   *key = NULL;
	void   *val = NULL;
	int		oldslots = hash->slots;
	ik_vec *oldkey = hash->key;
	ik_vec *oldval = hash->val;
	ik_vec	kvec;
	ik_vec	vvec;
	ik_tvec	keys;

	// create the new hash
	hash->level = hash->level +1;
	hash->slots = HashLevelToSlots(hash->level);
	hash->key	= ik_malloc(hash->slots * sizeof(struct ik_VEC));
	hash->val	= ik_malloc(hash->slots * sizeof(struct ik_VEC));
	for (i = 0; i < hash->slots; i++) {
		hash->key[i] = ik_vec_new();
		hash->val[i] = ik_vec_new();
	}

	// brand new hash?
	if (hash->keys->size == 0) return;

	keys = hash->keys;
	hash->keys = ik_tvec_new();

	// transfer old stuff to new hash
	for (i = 0; i < oldslots; i++) {
		kvec = oldkey[i];
		vvec = oldval[i];
		for (j = 0; j < kvec->size; j++) {
			key = kvec->elem[j];
			val = vvec->elem[j];
			ik_map_set(hash, key, val);
		}
	}

	// free old stuff
	for (i = 0; i < oldslots; i++) {
		kvec = oldkey[i];
		vvec = oldval[i];
		ik_vec_free(kvec);
		ik_vec_free(vvec);
	}
	free(oldkey);
	free(oldval);
	ik_tvec_free(keys);
}

void ik_map_free(ik_map hash) {
	int i;

	if (hash == NULL) return;

	for (i = 0; i < hash->slots; i++) {
		if (hash->key[i]) ik_vec_free(hash->key[i]);
		if (hash->val[i]) ik_vec_free(hash->val[i]);
	}
	ik_tvec_free(hash->keys);
	ik_vec_free(hash->vals);
	free(hash->key);
	free(hash->val);
	free(hash);
}

ik_map ik_map_new(void) {
	ik_map hash = ik_malloc(sizeof(struct ik_MAP));
	hash->level = 0;
	hash->slots = 0;
	hash->keys	= ik_tvec_new();
	hash->vals	= ik_vec_new();
	hash->key	= NULL;
	hash->val	= NULL;
	ExpandHash(hash);
	return hash;
}

void * ik_map_get(const ik_map hash, const char *key) {
	int	  i, index;
	char *string = NULL;

	index = HashFunc(hash, key);

	// resolve collisions
	for (i = 0; i < hash->key[index]->size; i++) {
		string = hash->key[index]->elem[i];
		if (strcmp(key, string) == 0) {
			return hash->val[index]->elem[i];
		}
	}
	return NULL; // return is NULL if not found
}

void ik_map_set(ik_map hash, const char *key, void *val) {
	int	  i, index;
	char *string = NULL;
	int	  new_key = 1;

	index = HashFunc(hash, key);

	// reassign unless new key
	for (i = 0; i < hash->key[index]->size; i++) {
		string = hash->key[index]->elem[i];
		if (strcmp(key, string) == 0) {
			hash->val[index]->elem[i] = val;
			new_key = 0;
			return;
		}
	}

	if (new_key) {
		ik_tvec_push(hash->keys, key);
		ik_vec_push(hash->key[index], hash->keys->last);
		ik_vec_push(hash->vals, val);
		ik_vec_push(hash->val[index], hash->vals->last);
	}

	// check if we have to expand the hash
	if ((float)hash->keys->size / (float)hash->slots >= MAX_HASH_DEPTH) {
		ExpandHash(hash);
	}
}

ik_tvec ik_map_keys(const ik_map hash) {
	int		i;
	ik_tvec vec = ik_tvec_new();
	for (i = 0; i < hash->keys->size; i++) ik_tvec_push(vec, hash->keys->elem[i]);
	return vec;
}

ik_vec ik_map_vals(const ik_map hash) {
	int	   i;
	ik_vec vec = ik_vec_new();
	for (i = 0; i < hash->vals->size; i++) ik_vec_push(vec, hash->vals->elem[i]);
	return vec;
}

void ik_map_stat(const ik_map hash) {
	int i, max, min, total, count;
	max = 0;
	min = 1000000;
	total = 0;
	for (i = 0; i < hash->slots; i++) {
		count = hash->val[i]->size;
		total += count;
		if (count > max) max = count;
		if (count < min) min = count;
	}
	fprintf(stdout, "HashStats: level=%d slots=%d keys=%d min=%d max=%d ave=%f\n",
		 hash->level, hash->slots, hash->keys->size, min, max,
		 (float)total / (float)hash->slots);
}

// text map

void ik_tmap_free(ik_tmap t) {
	ik_map_free(t->hash);
	ik_tvec_free(t->tvec);
	free(t);
}

ik_tmap ik_tmap_new(void) {
	ik_tmap t = ik_malloc(sizeof(struct ik_TMAP));
	t->hash = ik_map_new();
	t->tvec = ik_tvec_new();
	return t;
}

void ik_tmap_set(ik_tmap t, const char *key, const char *val) {
	ik_tvec_push(t->tvec, val);
	ik_map_set(t->hash, key, t->tvec->last);
}

int ik_tmap_exists(const ik_tmap t, const char *key) {
	void *ref = ik_map_get(t->hash, key);
	if (ref == NULL) return 0;
	return 1;
}

char * ik_tmap_get(const ik_tmap t, const char *key) {
	void *ref = ik_map_get(t->hash, key);
	assert(ref != NULL);
	return ref;
}

ik_tvec ik_tmap_keys(const ik_tmap t) {
	return ik_map_keys(t->hash);
}

// command line options

static ik_tvec COMMAND_LINE = NULL;
static ik_map CL_REGISTER	= NULL;
static ik_map CL_OPTIONS	= NULL;

void ik_register_option(const char *name, int flag) {
	if (COMMAND_LINE == NULL) {
		COMMAND_LINE = ik_tvec_new();
		CL_REGISTER	 = ik_map_new();
		CL_OPTIONS	 = ik_map_new();
	}

	switch (flag) {
		case 0: ik_map_set(CL_REGISTER, name, (void *)1); break;
		case 1: ik_map_set(CL_REGISTER, name, (void *)2); break;
		default: ik_exit("ik_register_option: flag 0 or 1");
	}
}

void ik_parse_options(int *argc, char **argv) {
	int i;
	char *token = NULL;

	for (i = 0; i < *argc; i++) {
		token = argv[i];
		if (token[0] == '-' && strlen(token) > 1) {
			switch ((size_t)ik_map_get(CL_REGISTER, token)) {
				case 0:
					ik_exit("unknown option (%s)", token);
					break;
				case 1:
					ik_map_set(CL_OPTIONS, token, token);
					break;
				case 2:
					ik_map_set(CL_OPTIONS, token, argv[i+1]);
					i++;
					break;
				default:
					ik_exit("not possible");
			}
		} else {
			ik_tvec_push(COMMAND_LINE, argv[i]);
		}
	}

	*argc = COMMAND_LINE->size;
	for (i = 0; i < COMMAND_LINE->size; i++) {
		argv[i] = COMMAND_LINE->elem[i];
	}
}

char * ik_option(const char *tag) {
	return ik_map_get(CL_OPTIONS, tag);
}

// pipe

void ik_pipe_close(ik_pipe pipe) {
	pipe->mode = 0;
	free(pipe->name);
	if (pipe->gzip) pclose(pipe->stream);
	else			fclose(pipe->stream);
	pipe->gzip = 0;
	free(pipe);
}

ik_pipe ik_pipe_open(const char *name, const char *mode) {
	char	command[1024];
	int     length = strlen(name);
	ik_pipe pipe = ik_malloc(sizeof(struct ik_PIPE));

	if		(strcmp(mode, "r") == 0)  pipe->mode = 0;
	else if (strcmp(mode, "w") == 0)  pipe->mode = 1;
	else if (strcmp(mode, "r+") == 0) pipe->mode = 2;
	else ik_exit("r, w, or r+ only in ik_pipe");

	pipe->name = ik_malloc(length + 1);
	strcpy(pipe->name, name);

	pipe->gzip = 0;

	if (name[length -3] == '.' &&
		name[length -2] == 'g' &&
		name[length -1] == 'z') pipe->gzip = 1; // .gz
	if (name[length -2] == '.' &&
		name[length -1] == 'z') pipe->gzip = 1; // .z
	if (name[length -2] == '.' &&
		name[length -1] == 'Z') pipe->gzip = 1; // .Z

	if (pipe->gzip) {
		if (pipe->mode != 0) ik_exit("compressed pipes are read only");
		sprintf(command, "gunzip -c %s", name);
		pipe->stream = popen(command, "r");
	} else {
		if (strcmp(name, "-") == 0) pipe->stream = stdin;
		else						pipe->stream = fopen(name, mode);
	}

	if (pipe->stream == NULL) {
		ik_exit("failed to open %s\n", name);
	}

	return pipe;
}

#endif
