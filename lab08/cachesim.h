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

typedef struct {
    unsigned int index;
    unsigned int tag;
} cache_idx;

#endif
