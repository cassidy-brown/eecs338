/* Heap data structure
 * Code copied from Jason Marcell: https://github.com/jasmarc/scheduler
 * I promise I only directly copied the heap, which looks like it's from a textbook anyway. The rest of his repo was only used for inspiration
 */

#include <stdio.h>
#include <stdarg.h>

#define MAX_HEAP_SIZE 1000
#define PARENT(i) (i/2)     // given the index of a heap node, the returns the parent index
#define LEFT(i) (2*i)       // given the index of a heap node, the returns the left child index
#define RIGHT(i) (2*i + 1)  // given the index of a heap node, the returns the right child index
#define SWAP(a, b, temp)  {temp=a; a=b; b=temp;}

typedef struct {
    void *a[MAX_HEAP_SIZE];
    int size;
} heap;

void heap_init(heap *h);
void heapify(heap *h, int (*comp_func)(void*, void*), int i);
void build_heap(heap *h, int (*comp_func)(void*, void*));
void* heap_extract_max(heap *h, int (*comp_func)(void*, void*));
void heap_insert(heap *h, int (*comp_func)(void*, void*), void *key);

// set heap size to 0 and NULL out all values
void heap_init(heap *h)
{
    int j;
    h->size = 0;
    for(j = 0; j < MAX_HEAP_SIZE; j++)
        h->a[j] = NULL;
    return;
}

// Adapted from Introduction to Algorithms, 1990, 
// by Cormen, Leiserson, Rivest, and Stein.
// Chapter 7, Section 7.2
void heapify(heap *h, int (*comp_func)(void*, void*), int i)
{
    void *temp = NULL;
    int largest,
        l = LEFT(i),
        r = RIGHT(i);
    if(l <= h->size && comp_func(h->a[l], h->a[i]) > 0)
        largest = l;
    else
        largest = i;
    if(r <= h->size && comp_func(h->a[r], h->a[largest]) > 0)
        largest = r;
    if(largest != i) {
        SWAP(h->a[i], h->a[largest], temp);
        heapify(h, comp_func, largest);
    }
    return;
}

// Adapted from Introduction to Algorithms, 1990, 
// by Cormen, Leiserson, Rivest, and Stein.
// Chapter 7, Section 7.3
void build_heap(heap *h, int (*comp_func)(void*, void*))
{
    int i;
    for(i = h->size/2; i > 0; i--)
        heapify(h, comp_func, i);
    return;
}

// Adapted from Introduction to Algorithms, 1990, 
// by Cormen, Leiserson, Rivest, and Stein.
// Chapter 7, Section 7.5
void* heap_extract_max(heap *h, int (*comp_func)(void*, void*))
{
    if(h->size < 1)
        return NULL;
    void *max = h->a[1],
         *temp = NULL;
    SWAP(h->a[1], h->a[h->size], temp);
    h->size--;
    heapify(h, comp_func, 1);
    return max;
}

// Adapted from Introduction to Algorithms, 1990, 
// by Cormen, Leiserson, Rivest, and Stein.
// Chapter 7, Section 7.5
void heap_insert(heap *h, int (*comp_func)(void*, void*), void *key)
{
    int i = ++h->size;
    while(i > 1 && comp_func(h->a[PARENT(i)], key) < 0) {
        h->a[i] = h->a[PARENT(i)];
        i = PARENT(i);
    }
    h->a[i] = key;
    return;
}