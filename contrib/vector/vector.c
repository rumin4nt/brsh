//
//  vector.c
//  r4
//
//  Created by Michael Patraw.
//

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

#define GROWTH_RATE 1.6
//#define VECTOR_ERROR_NOMEM 8

static void *aligned_malloc(size_t align, size_t size)
{
	void *mem = malloc(size + (align - 1) + sizeof(void *));
	if (!mem) {
		return NULL;
	}
	char *amem = ((char *)mem) + sizeof(void *);
	amem += (align - ((uintptr_t)amem & (align - 1)) & (align - 1));
	((void **)amem)[-1] = mem;
	return amem;
}

static void aligned_free(void *mem)
{
	if (!mem) {
		return;
	}
	free(((void **)mem)[-1]);
}

static int alloc_to_size(struct vector_t *v, size_t size)
{
	assert(v->length * v->elem_size <= size);
	void *new_data = aligned_malloc(v->elem_size, size);
	if (!new_data) {
		return VECTOR_ERROR_NOMEM;
	}
	v->allocd = size;
	memcpy(new_data, v->data, v->length * v->elem_size);
	aligned_free(v->data);
	v->data = new_data;
	return 0;
}

static int grow(struct vector_t *v)
{
	size_t max_elems = v->allocd / v->elem_size;
	size_t new_max = max_elems * GROWTH_RATE;
	if (new_max <= max_elems) {
		++new_max;
	}
	return alloc_to_size(v, new_max * v->elem_size);
}

int vector_init(struct vector_t *v, size_t initial, size_t elem_size)
{
	v->allocd = 0;
	v->length = 0;
	v->elem_size = elem_size;
	v->data = aligned_malloc(elem_size, initial * elem_size);
	if (!v->data) {
		return VECTOR_ERROR_NOMEM;
	}
	v->allocd = initial * elem_size;
	return 0;
}

int vector_shrink(struct vector_t *v)
{
	return alloc_to_size(v, v->length * v->elem_size);
}

int vector_reserve(struct vector_t *v, size_t size)
{
	v->length = v->length < size ? size : v->length;
	return alloc_to_size(v, size * v->elem_size);
}

void vector_uninit(struct vector_t *v)
{
	if (v->data) {
		aligned_free(v->data);
	}
	memset(v, 0, sizeof(*v));
}

int vector_push_back(struct vector_t *v, void *data)
{
	if ((v->length + 1) * v->elem_size >= v->allocd) {
		int rv = grow(v);
		if (rv != 0) {
			return rv;
		}
	}
	char *array = v->data;
	memcpy(&array[v->length * v->elem_size], data, v->elem_size);
	++v->length;
	return 0;
}

int vector_push_front(struct vector_t *v, void *data)
{
	if ((v->length + 1) * v->elem_size >= v->allocd) {
		int rv = grow(v);
		if (rv != 0) {
			return rv;
		}
	}
	char *array = v->data;
	memmove(&array[v->elem_size], array, v->length * v->elem_size);
	memcpy(&array[0], data, v->elem_size);
	++v->length;
	return 0;
}

void vector_pop_back(struct vector_t *v, void *out)
{
	if (out) {
		char *array = v->data;
		memcpy(out, &array[(v->length - 1) * v->elem_size], v->elem_size);
	}
	--v->length;
}

void vector_pop_front(struct vector_t *v, void *out)
{
	char *array = v->data;
	memmove(array, &array[v->elem_size], (v->length - 1) * v->elem_size);
	if (out) {
		memcpy(out, &array[0], v->elem_size);
	}
	--v->length;
}

void vector_get(struct vector_t *v, size_t idx, void *out)
{
	char *array = v->data;
	memcpy(out, &array[idx * v->elem_size], v->elem_size);
}

void vector_set(struct vector_t *v, size_t idx, void *data)
{
	char *array = v->data;
	memcpy(&array[idx * v->elem_size], data, v->elem_size);
}

void vector_del(struct vector_t *v, size_t idx, void *out)
{
	char *array = v->data;
	if (out) {
		memcpy(out, &array[idx * v->elem_size], v->elem_size);
	}
	size_t leftover = (v->length - idx - 1) * v->elem_size;
	memmove(&array[idx * v->elem_size], &array[(idx + 1) * v->elem_size], leftover);
	--v->length;
}

#ifdef UNITTEST
#include <stdio.h>
int main(void)
{
	struct vector_t v;
	if (vector_init(&v, 0, sizeof(int)) == -1) {
		fprintf(stderr, "error: out of memory\n");
		return -1;
	}
	
	for (int i = 0; i < 10; ++i) {
		vector_push_back(&v, &i);
	}
	int in = 42;
	vector_set(&v, 5, &in);
	
	int out;
	vector_get(&v, 6, &out);
	printf("out=%d\n", out);
	vector_get(&v, 5, &out);
	printf("out=%d\n", out);
	
	vector_del(&v, 5, NULL);
	vector_pop_front(&v, NULL);
	vector_push_front(&v, &in);
	vector_shrink(&v);
	for (int i = 0; i < v.length; ++i) {
		vector_get(&v, i, &out);
		printf("[%d]=%d\n", i, out);
	}
	
	printf("v=%zu,%zu,%zu\n", v.allocd, v.length, v.elem_size);
	
	vector_uninit(&v);
}
#endif
