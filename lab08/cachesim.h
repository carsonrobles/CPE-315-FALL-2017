#ifndef CACHESIM_H
#define CACHESIM_H

#define MAX_CACHE 256
#define MAX_ASSOC 8

typedef struct {
    unsigned int hits;
    unsigned int misses;
    unsigned int reads;
    unsigned int writes;
    unsigned int assoc;
} stat_t;

typedef struct {
    unsigned int tag;
    int data;
} line_t;

typedef struct {
    unsigned int next_in;
    line_t lines[MAX_ASSOC];
} block_t;

typedef struct {
    unsigned int cachesize;
    unsigned int blocksize;
    unsigned int next_in;
    stat_t stats;
    block_t cache[MAX_CACHE];
} cache_t;



#endif
