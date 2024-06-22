CXX = g++
CXXFLAGS = -std=c++11 -Wall
SRC_DIR = .
OBJ_DIR = obj
EXE = library_system

# List all source files in the current directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding object file names
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	del /S /Q obj
	del /S /Q library_system.exe

