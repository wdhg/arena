#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct arena Arena;

Arena *ArenaAlloc(void);
void ArenaFree(Arena *arena);
void *ArenaPush(Arena *arena, size_t size);
void *ArenaPushZero(Arena *arena, size_t size);

#define ArenaPushArray(arena, count, type)                                     \
	(type *)ArenaPush((arena), (count) * sizeof(type))
#define ArenaPushArrayZero(arena, count, type)                                 \
	(type *)ArenaPushZero((arena), (count) * sizeof(type))

#define ArenaPushStruct(arena, type) (type *)ArenaPush((arena), sizeof(type))
#define ArenaPushStructZero(arena, type)                                       \
	(type *)ArenaPushZero((arena), sizeof(type))

#endif
