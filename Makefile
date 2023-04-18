DIR_SRC = src
DIR_OBJ = obj
DIR_OBJ_LIB = $(DIR_OBJ)/lib
DIR_OBJ_DEBUG = $(DIR_OBJ)/debug

C_FLAGS  = -Wall -Wextra -pedantic -std=c89
C_FLAGS += -Iinclude

.PHONY: lib
lib: $(DIR_OBJ_LIB)/arena.o

.PHONY: debug
debug: $(DIR_OBJ_DEBUG)/arena.o

$(DIR_OBJ_LIB)/%.o: $(DIR_SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(C_FLAGS) -O3

$(DIR_OBJ_DEBUG)/%.o: $(DIR_SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(C_FLAGS) -g

.PHONY: clean
clean:
	rm -rf $(DIR_OBJ)
