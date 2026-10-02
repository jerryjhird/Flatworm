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
	@comma=""; \
	for src in $(SRCS); do \
		rel_path="$${src#$(SRC_DIR)/}"; \
		obj="$(BUILD_DIR)/$${rel_path%.*}.o"; \
		printf "%s\n  {\n    \"directory\": \"%s\",\n    \"command\": \"%s %s -c %s -o %s\",\n    \"file\": \"%s\",\n    \"output\": \"%s\"\n  }" \
			"$$comma" "$(CURDIR)" "$(CC)" "$(subst ",\",$(CFLAGS))" "$$src" "$$obj" "$$src" "$$obj" >> compile_commands.json; \
		comma=","; \
	done
	@printf "\n]\n" >> compile_commands.json

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

git:
	./format

clean:
	rm -rf build $(TARGET) compile_commands.json

.PHONY: all clean
