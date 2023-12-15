CC := gcc

CFLAGS := -Wall -Wextra -I./src/include

# Source files
SRC_DIR := ./src
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_DIR := ./build
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

EXECUTABLE := TicTacToe

# Create the obj directory if it doesn't exist
$(shell mkdir -p $(OBJ_DIR))

# Build executable
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Phony target to clean the project
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)