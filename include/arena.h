#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

struct arena;

struct arena *arena_alloc(void);
void arena_free(struct arena *arena);
void *arena_push(struct arena *arena, size_t size);
void *arena_push_zero(struct arena *arena, size_t size);

#define arena_push_array(arena, count, type)                                   \
	(type *)arena_push((arena), (count) * sizeof(type))
#define arena_push_array_zero(arena, count, type)                              \
	(type *)arena_push_zero((arena), (count) * sizeof(type))

#define arena_push_struct(arena, type) (type *)arena_push((arena), sizeof(type))
#define arena_push_struct_zero(arena, type)                                    \
	(type *)arena_push_zero((arena), sizeof(type))

#endif
