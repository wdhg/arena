DIR_SRC = src
DIR_LIB = lib
DIR_OBJ = obj

C_FLAGS  = -Wall -Wextra -pedantic -std=c89
C_FLAGS += -Iinclude

.PHONY: lib
lib: $(DIR_OBJ)/arena.o

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(C_FLAGS) -O3

.PHONY: clean
clean:
	rm -rf $(DIR_BIN) $(DIR_OBJ)
