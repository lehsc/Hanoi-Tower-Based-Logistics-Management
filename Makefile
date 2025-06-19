CC = g++
CFLAGS = -std=c++11 -g -Wall

# Folders
INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

# All sources, objs, and header files
SRC_FILES = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ_FILES = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC_FILES))

# Executable
TARGET = $(BIN_FOLDER)tp2.out

# Rules
all: $(BIN_FOLDER) $(OBJ_FOLDER) $(TARGET)

$(BIN_FOLDER) $(OBJ_FOLDER):
	mkdir -p $@

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_FOLDER)* $(BIN_FOLDER)*
