
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define VECTOR_ERROR_NOMEM (-1)

struct vector_t
{
	/* Allocated space in bytes. */
	size_t allocd;
	/* Current length of vector. */
	size_t length;
	/* Size of each element in bytes. */
	size_t elem_size;
	/* Aligned raw byte array. Can only be free'd internally. */
	void *data;
};

int vector_init(struct vector_t *v, size_t initial, size_t elem_size);
void vector_uninit(struct vector_t *v);
int vector_shrink(struct vector_t *v);
int vector_reserve(struct vector_t *v, size_t size);
int vector_push_back(struct vector_t *v, void *data);
int vector_push_front(struct vector_t *v, void *data);
void vector_pop_back(struct vector_t *v, void *out);
void vector_pop_front(struct vector_t *v, void *out);
void vector_get(struct vector_t *v, size_t idx, void *out);
void vector_set(struct vector_t *v, size_t idx, void *data);
void vector_del(struct vector_t *v, size_t idx, void *out);

#endif
