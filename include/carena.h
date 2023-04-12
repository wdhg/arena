#ifndef ARENA_H
#define ARENA_H

#include <assert.h>
#include <stdlib.h>

#define INITIAL_ARENA_SIZE 4096

struct ca_arena {
	size_t cap; /* capacity, max number of bytes */
	size_t len; /* length, number of allocated bytes */
	char *stack;
};

struct ca_arena *ca_arena_alloc(void) {
	struct ca_arena *arena =
			(struct ca_arena *)calloc(1, sizeof(struct ca_arena));
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

struct ca_arena *ca_arena_grow(struct ca_arena *arena) {
	size_t cap_new = arena->cap * 2;
	char *stack_new = (char *)realloc(arena->stack, cap_new);
	if (stack_new == NULL) {
		return NULL;
	}
	arena->stack = stack_new;
	arena->cap = cap_new;
	return arena;
}

void ca_arena_free(struct ca_arena *arena) {
	assert(arena != NULL);
	assert(arena->stack != NULL);
	free(arena->stack);
	free(arena);
}

void *ca_arena_push(struct ca_arena *arena, size_t size) {
	void *ptr = &arena->stack[arena->len];
	assert(arena != NULL);
	while (size > arena->cap - arena->len) {
		arena = ca_arena_grow(arena);
		if (arena == NULL) {
			return NULL;
		}
	}
	arena->len += size;
	return ptr;
}

void *ca_arena_push_zero(struct ca_arena *arena, size_t size) {
	char *ptr = (char *)ca_arena_push(arena, size);
	size_t i;
	if (ptr == NULL) {
		return NULL;
	}
	for (i = 0; i < size; i++) {
		ptr[i] = 0;
	}
	return ptr;
}

#define ca_arena_push_array(arena, count, type)                                \
	(type *)ca_arena_push((arena), (count) * sizeof(type))
#define ca_arena_push_array_zero(arena, count, type)                           \
	(type *)ca_arena_push_zero((arena), (count) * sizeof(type))

#define ca_arena_push_struct(arena, type)                                      \
	(type *)ca_arena_push((arena), sizeof(type))
#define ca_arena_push_struct_zero(arena, type)                                 \
	(type *)ca_arena_push_zero((arena), sizeof(type))

#endif
