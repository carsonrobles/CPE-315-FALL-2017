#ifndef CACHESIM_H
#define CACHESIM_H

typedef struct {
    unsigned int hits;
    unsigned int misses;
    unsigned int reads;
    unsigned int writes;
    unsigned int assoc;
    unsigned int size;
} stats;

typedef unsigned int block[4];
typedef struct {
    unsigned int cachesize;
    unsigned int blocksize;
    block cache[256];
} cache_t;

typedef struct {
    unsigned int index;
    unsigned int tag;
} cache_idx;

#endif
