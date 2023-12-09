#include <arena.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t u8;

/* 1 MiB */
#define MIN_ARENA_SIZE (1 << 20)

typedef struct stack Stack;

struct stack {
	size_t cap; /* capacity, max number of bytes */
	size_t len; /* length, number of allocated bytes */
	u8 *bytes;
	Stack *next;
};

struct arena {
	Stack *head;
	Stack *last;
};

Stack *stack_alloc(size_t cap) {
	Stack *stack = (Stack *)calloc(1, sizeof(Stack));
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

int arena_append_stack(Arena *arena, size_t cap) {
	Stack *new_stack;
	assert(arena != NULL);
	new_stack = stack_alloc(cap);
	if (new_stack == NULL) {
		return 0;
	}
	if (arena->last == NULL) {
		arena->head = new_stack;
	} else {
		arena->last->next = new_stack;
	}
	arena->last = new_stack;
	return 1;
}

Arena *arena_alloc(void) {
	Arena *arena = (Arena *)calloc(1, sizeof(Arena));
	if (arena == NULL) {
		return NULL;
	}
	return arena;
}

void stacks_free(Stack *stack) {
	assert(stack != NULL);
	assert(stack->bytes != NULL);
	if (stack->next != NULL) {
		stacks_free(stack->next);
	}
	free(stack->bytes);
	free(stack);
}

void arena_free(Arena *arena) {
	assert(arena != NULL);
	stacks_free(arena->head);
	free(arena);
}

int stack_can_fit(Stack *stack, size_t size) {
	assert(stack != NULL);
	return (size <= stack->cap - stack->len);
}

void *stack_push(Stack *stack, size_t size) {
	void *ptr;
	assert(stack != NULL);
	assert(stack_can_fit(stack, size));
	ptr = &stack->bytes[stack->len];
	stack->len += size;
	return ptr;
}

void *arena_push(Arena *arena, size_t size) {
	assert(arena != NULL);
	if (arena->last == NULL || !stack_can_fit(arena->last, size)) {
		size_t new_stack_cap = size < MIN_ARENA_SIZE ? MIN_ARENA_SIZE : size;
		if (!arena_append_stack(arena, new_stack_cap)) {
			return NULL;
		}
	}
	return stack_push(arena->last, size);
}

void *arena_push_zero(Arena *arena, size_t size) {
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
