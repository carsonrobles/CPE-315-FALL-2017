#ifndef CACHESIM_H
#define CACHESIM_H

typedef struct {
    unsigned int hits;
    unsigned int misses;
    unsigned int reads;
    unsigned int writes;
    unsigned int assoc;
} stat_t;

typedef struct {
    unsigned int tag;
    unsigned int data;
} line;

typedef line block[8];

typedef struct {
    unsigned int cachesize;
    unsigned int blocksize;
    unsigned int next_in;
    state_t stats;
    block cache[256];
} cache_t;



#endif
