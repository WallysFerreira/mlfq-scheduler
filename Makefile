BUILD_DIR := build
SRC_DIR := src

SRC_FILES := $(SRC_DIR)/main.c

OBJ_FILES := $(BUILD_DIR)/main.o

all: ${BUILD_DIR}/simulator

# Link - Combine all into a file
${BUILD_DIR}/simulator: ${OBJ_FILES}
	cc $^ -o $@

# Compile - Make objects
${BUILD_DIR}/%.o: ${SRC_DIR}/%.c | ${BUILD_DIR}
	cc -c $< -o $@

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}