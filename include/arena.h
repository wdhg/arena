#ifndef ARENA_H
#define ARENA_H

#include <assert.h>
#include <stdlib.h>

#define INITIAL_ARENA_SIZE 4096

struct arena {
	size_t cap; /* capacity, max number of bytes */
	size_t len; /* length, number of allocated bytes */
	char *stack;
};

struct arena *arena_alloc(void) {
	struct arena *arena = (struct arena *)calloc(1, sizeof(struct arena));
	if (arena == NULL) {
		return NULL;
	}
	arena->stack = (char *)calloc(INITIAL_ARENA_SIZE, sizeof(char));
	if (arena->stack == NULL) {
		free(arena);
		return NULL;
	}
	return arena;
}

struct arena *arena_grow(struct arena *arena) {
	size_t cap_new = arena->cap * 2;
	char *stack_new = (char *)realloc(arena->stack, cap_new);
	if (stack_new == NULL) {
		return NULL;
	}
	arena->stack = stack_new;
	arena->cap = cap_new;
	return arena;
}

void arena_free(struct arena *arena) {
	assert(arena != NULL);
	assert(arena->stack != NULL);
	free(arena->stack);
	free(arena);
}

void *arena_push(struct arena *arena, size_t size) {
	void *ptr = &arena->stack[arena->len];
	assert(arena != NULL);
	while (size > arena->cap - arena->len) {
		arena = arena_grow(arena);
		if (arena == NULL) {
			return NULL;
		}
	}
	arena->len += size;
	return ptr;
}

void *arena_push_zero(struct arena *arena, size_t size) {
	char *ptr = (char *)arena_push(arena, size);
	size_t i;
	if (ptr == NULL) {
		return NULL;
	}
	for (i = 0; i < size; i++) {
		ptr[i] = 0;
	}
	return ptr;
}

#define arena_push_array(arena, count, type)                                   \
	(type *)arena_push((arena), (count) * sizeof(type))
#define arena_push_array_zero(arena, count, type)                              \
	(type *)arena_push_zero((arena), (count) * sizeof(type))

#define arena_push_struct(arena, type) (type *)arena_push((arena), sizeof(type))
#define arena_push_struct_zero(arena, type)                                    \
	(type *)arena_push_zero((arena), sizeof(type))

#endif
