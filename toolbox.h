/******************************************************************************\
 toolbox.h
 Copyright (C) Ian Korf
\******************************************************************************/

#ifndef IK_TOOLBOX_H
#define IK_TOOLBOX_H

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// library and program info
char * ik_get_version_number(void);
void   ik_set_program_name(const char *);
char * ik_get_program_name(void);

// specialized output
void ik_exit(const char *, ...);

// memory
void * ik_malloc(size_t);
void * ik_calloc(size_t, size_t);
void * ik_realloc(void *, size_t);

// integer vector
struct ik_IVEC {
	int * elem;
	int   size;
	int   limit;
	int   last;
};
typedef struct ik_IVEC * ik_ivec;
void	ik_ivec_free(ik_ivec);
ik_ivec ik_ivec_new(void);
void	ik_ivec_push(ik_ivec, int);
int     ik_ivec_pop(ik_ivec);

// float vector
struct ik_FVEC {
	float * elem;
	int     size;
	int     limit;
	int     last;
};
typedef struct ik_FVEC * ik_fvec;
void	ik_fvec_free(ik_fvec);
ik_fvec ik_fvec_new(void);
void	ik_fvec_push(ik_fvec, float);
float   ik_fvec_pop(ik_fvec);

// text vector
struct ik_TVEC {
	char ** elem;
	int     size;
	int     limit;
	char  * last;
};
typedef struct ik_TVEC * ik_tvec;
void	ik_tvec_free(ik_tvec);
ik_tvec ik_tvec_new(void);
void	ik_tvec_push(ik_tvec, const char *);
char *  ik_tvec_pop(ik_tvec);

// generic void * vector
struct ik_VEC {
	void ** elem;
	int     size;
	int     limit;
	void  * last;
};
typedef struct ik_VEC * ik_vec;
void   ik_vec_free(ik_vec);
ik_vec ik_vec_new(void);
void   ik_vec_push(ik_vec, void *);
void * ik_vec_pop(ik_vec);

// generic map (text key, void * value)
struct ik_MAP {
	int      level;
	int      slots;
	ik_tvec	 keys;
	ik_vec	 vals;
	ik_vec * key;
	ik_vec * val;
};
typedef struct ik_MAP * ik_map;
void	ik_map_free(ik_map);
ik_map	ik_map_new(void);
void	ik_map_set(ik_map, const char *, void *);
void *	ik_map_get(const ik_map, const char *);
ik_tvec ik_map_keys(const ik_map);
ik_vec	ik_map_vals(const ik_map);
void	ik_map_stat(const ik_map);

// text map
struct ik_TMAP {
	ik_map  hash;
	ik_tvec tvec;
};
typedef struct ik_TMAP * ik_tmap;
void    ik_tmap_free(ik_tmap);
ik_tmap ik_tmap_new(void);
void    ik_tmap_set(ik_tmap, const char *, const char *);
char *  ik_tmap_get(const ik_tmap, const char *);
int     ik_tmap_exists(const ik_tmap, const char *);
ik_tvec ik_tmap_keys(const ik_tmap);

// command line processing
void   ik_register_option(const char *, int);
void   ik_parse_options(int *, char **);
char * ik_option(const char *);

// pipe
struct ik_PIPE {
	int    mode; // 0 = read, 1 = write, 2 = r+
	char * name;
	int    gzip;
	FILE * stream;
};
typedef struct ik_PIPE * ik_pipe;
void    ik_pipe_free(ik_pipe);
ik_pipe ik_pipe_open(const char *, const char *);
void    ik_pipe_close(ik_pipe);

/* generic suffix tree */
struct ik_xnode {
	ik_vec children;
	void  *data;
	char   c;
};
typedef struct ik_xnode * ik_xnode; /* note: all methods private */
ik_xnode ik_xnode_new(char);
ik_xnode ik_xnode_search(const ik_xnode, char c);

struct ik_xtree {
	ik_xnode head;
	ik_vec   alloc;
};
typedef struct ik_xtree * ik_xtree;
void     ik_xtree_free(ik_xtree);
ik_xtree ik_xtree_new(void);
void *   ik_xtree_get(const ik_xtree, const char *);
int      ik_xtree_check(const ik_xtree, const char *);
ik_xnode ik_xtree_node(const ik_xtree, const char *);
void     ik_xtree_set(ik_xtree, const char *, void *);
ik_tvec	 ik_xtree_keys(const ik_xtree);

#endif
