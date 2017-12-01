/* Lab 8
 * Dane Mortensen and Carson Robles */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cachesim.h"

#define AMAX 10         /* Maximum (square) array size */
#define CACHESIM 1      /* Set to 1 if simulating Cache */

unsigned int tag_search(block_t *b, unsigned int blocksize, unsigned int tag) {
    int l;
    for (l = 0; l < blocksize; l++) {
        if (b->lines[l].tag == tag)
            return l;
    }
    return -1;
}

void access_cache(int *mp, cache_t *c) {
    uintptr_t address = (uintptr_t) mp;
    unsigned int index, tag;

    if (c->cachesize == 16)
        index = address & 0xf;    /* index = four lowest order bits */
    else    /* otherwise, cache size = 256 */
        index = address & 0xff;

    if (tag_search(&c->cache[index], c->blocksize, address) == -1) {
        c->stats.misses++;
        c->cache[index].lines[c->cache[index].next_in].data = *mp;
        c->cache[index].lines[c->cache[index].next_in].tag = address;
        c->cache[index].next_in = (c->cache[index].next_in + 1) % c->blocksize;
    } else {
        c->stats.hits++;
    }
}

/* This function gets called with each "read" reference to memory */
void mem_read(int *mp, cache_t *c) {
    /* printf("Memory read from location %p\n", mp); */
    access_cache(mp, c);
    c->stats.reads++;
}

/* This function gets called with each "write" reference to memory */
void mem_write(int *mp, cache_t *c) {
    /* printf("Memory write to location %p\n", mp); */
    access_cache(mp, c);
    c->stats.writes++;
}

/* Statically define the arrays a, b, and mult, where mult will become the
 * cross product of a and b, i.e., a x b. */
static int a[AMAX][AMAX], b[AMAX][AMAX], mult[AMAX][AMAX];

void matmul(cache_t *c, r1, c1, c2) {
    int i, j, k;
    int *mp1, *mp2, *mp3;

    /* Initializing elements of matrix mult to 0.*/
    for(i=0; i<r1; ++i)
        for(j=0; j<c2; ++j) {
            mult[i][j]=0;
        }

/* Multiplying matrix a and b and storing in array mult. */
    for(i=0; i<r1; ++i)
        for(j=0; j<c2; ++j)
            for(k=0; k<c1; ++k) {

                #if CACHESIM        /* "Hooks" to measure memory references - enabled if CACHESIM  */
                mp1 = &mult[i][j];
                mp2 = &a[i][k];
                mp3 = &b[k][j];     
                mem_read(mp1, c);
                mem_read(mp2, c);
                mem_read(mp3, c);
                mem_write(mp1, c); 
                #endif

                mult[i][j]+=a[i][k]*b[k][j];

            }
}

int main() {
    int r1, c1, r2, c2, i, j, k, *mp1, *mp2, *mp3;

    cache_t c;
    memset(&c, 0, sizeof(cache_t));

    printf("Cache size (16 | 256): ");
    scanf("%u", &c.cachesize);
    while (c.cachesize != 16 && c.cachesize != 256) {
        printf("Invalid cache size\nCache size (16 | 256): ");
        scanf("%u", &c.cachesize);
    }

    printf("Accociativity (1 | 2 | 4): ");
    scanf("%u", &c.blocksize);
    while (c.blocksize != 1 && c.blocksize != 2 && c.blocksize != 4) {
        printf("Invalid block size\nAssociativity (1 | 2 | 4): ");
        scanf("%u", &c.blocksize);
    }

    printf("\nSize of pointer: %lu\n\n", sizeof(mp1));

    printf("Enter number of rows first matrix: ");
    scanf("%u", &r1);
    printf("Enter number of columns for first matrix: ");
    scanf("%u", &c1);
    printf("Enter number of rows for second matrix: ");
    scanf("%u",&r2);
    while (c1 != r2) {
        printf("ERROR: Column of first matrix not equal to row of second\n");
        printf("Enter number of rows first matrix: ");
        scanf("%u", &r1);
        printf("Enter number of columns for first matrix: ");
        scanf("%u", &c1);
        printf("Enter number of rows for second matrix: ");
        scanf("%u",&r2);
    }
    printf("Enter number of columns for second matrix: ");
    scanf("%u", &c2);

    /* Storing elements of first matrix. */
    printf("\nEnter elements of matrix A:\n");
    for(i=0; i<r1; ++i)
        for(j=0; j<c1; ++j) {
            /* printf("-\tEnter element A[%d, %d]: ",i+1,j+1);
             * scanf("%d",&a[i][j]); */
            a[i][j] = i+j; // build sample data
        }

/* Storing elements of second matrix. */
    printf("\nEnter elements of matrix B:\n");
    for(i=0; i<r2; ++i)
        for(j=0; j<c2; ++j) {
            /* printf("-\tEnter element B[%d, %d]: ",i+1,j+1);
             * scanf("%d",&b[i][j]); */
            b[i][j] = 10 + i + j;
        }

    matmul(&c, r1, c1, c2);        /* Invoke matrix multiply function */ 

    /* Displaying the multiplication of two matrix. */
    printf("\nOutput Matrix:\n");
    for(i=0; i<r1; ++i)
        for(j=0; j<c2; ++j) {
            printf("%d  ",mult[i][j]);
            if(j==c2-1)
                printf("\n\n");
        }

    printf("Read-write ratio: %u reads / %u writes = %.3f\n", c.stats.reads,\
            c.stats.writes, (float) c.stats.reads / c.stats.writes);
    printf("Hit-miss ratio: %u hits / %u misses = %.3f\n", c.stats.hits,\
            c.stats.misses, (float) c.stats.hits / c.stats.misses);

    return 0;
}
