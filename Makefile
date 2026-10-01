CC=clang
CFLAGS = -std=c11 -Wall -Wextra -Isrc -MMD -MP
LDFLAGS =

SRC_DIR = src
BUILD_DIR = build/srctree
TARGET = jruntime

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "[" > compile_commands.json
	@cat $(shell find $(BUILD_DIR) -name '*.o.json') | sed '$$!s/$$/,/' >> compile_commands.json
	@echo "]" >> compile_commands.json

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MJ $@.json -c $< -o $@

-include $(DEPS)

git:
	./format

clean:
	rm -rf build $(TARGET) compile_commands.json

.PHONY: all clean
