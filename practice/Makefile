.DEFAULT_GOAL := test

CC ?= cc
CFLAGS ?= -O2 -g -std=c11 -Wall -Wextra -Wpedantic -Werror
BUILD_DIR := build
TEST_BIN := $(BUILD_DIR)/test_add_u8
SOURCES := tests/test_add_u8.c src/add_u8.S

.PHONY: all test sanitize disasm debug clean

all: test

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@

sanitize: $(SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) -O1 -g -std=c11 -Wall -Wextra -Wpedantic -Werror \
		-fsanitize=address,undefined -fno-omit-frame-pointer $(SOURCES) \
		-o $(BUILD_DIR)/test_add_u8_sanitize
	./$(BUILD_DIR)/test_add_u8_sanitize

disasm: $(TEST_BIN)
	objdump -d -Mintel $(TEST_BIN)

debug: $(TEST_BIN)
	gdb --args ./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)
