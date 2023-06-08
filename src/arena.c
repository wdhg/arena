#include <arena.h>
#include <assert.h>
#include <fixint.h>
#include <stdlib.h>

/* 1 MiB */
#define MIN_ARENA_SIZE (1 << 20)

struct stack {
	size_t cap; /* capacity, max number of bytes */
	size_t len; /* length, number of allocated bytes */
	u8 *bytes;
	struct stack *next;
};

struct arena {
	struct stack *head;
	struct stack *last;
};

struct stack *stack_alloc(size_t cap) {
	struct stack *stack = (struct stack *)calloc(1, sizeof(struct stack));
	if (stack == NULL) {
		return NULL;
	}
	stack->cap   = cap;
	stack->len   = 0;
	stack->bytes = (u8 *)calloc(stack->cap, sizeof(u8));
	if (stack->bytes == NULL) {
		free(stack);
		return NULL;
	}
	return stack;
}

int arena_append_stack(struct arena *arena, size_t cap) {
	struct stack *new_stack;
	assert(arena != NULL);
	new_stack = stack_alloc(cap);
	if (new_stack == NULL) {
		return 0;
	}
	if (arena->last == NULL) {
		arena->head = new_stack;
		arena->last = new_stack;
	} else {
		arena->last->next = new_stack;
	}
	return 1;
}

struct arena *arena_alloc(void) {
	struct arena *arena = (struct arena *)calloc(1, sizeof(struct arena));
	if (arena == NULL) {
		return NULL;
	}
	return arena;
}

void stacks_free(struct stack *stack) {
	assert(stack != NULL);
	assert(stack->bytes != NULL);
	if (stack->next != NULL) {
		stacks_free(stack->next);
	}
	free(stack->bytes);
	free(stack);
}

void arena_free(struct arena *arena) {
	assert(arena != NULL);
	stacks_free(arena->head);
	free(arena);
}

void *stack_push(struct stack *stack, size_t size) {
	void *ptr;
	assert(stack != NULL);
	assert(size <= stack->cap - stack->len);
	ptr = &stack->bytes[stack->len];
	stack->len += size;
	return ptr;
}

int stack_can_fit(struct stack *stack, size_t size) {
	assert(stack != NULL);
	return (size < stack->cap - stack->len);
}

void *arena_push(struct arena *arena, size_t size) {
	assert(arena != NULL);
	if (arena->last == NULL || !stack_can_fit(arena->last, size)) {
		size_t new_stack_cap = size < MIN_ARENA_SIZE ? MIN_ARENA_SIZE : size;
		if (!arena_append_stack(arena, new_stack_cap)) {
			return NULL;
		}
	}
	return stack_push(arena->last, size);
}

void *arena_push_zero(struct arena *arena, size_t size) {
	u8 *ptr = (u8 *)arena_push(arena, size);
	size_t i;
	if (ptr == NULL) {
		return NULL;
	}
	for (i = 0; i < size; i++) {
		ptr[i] = 0;
	}
	return ptr;
}
