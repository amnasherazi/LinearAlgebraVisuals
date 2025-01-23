CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include -I/opt/homebrew/include/GLFW -Wall
LDFLAGS = -L/opt/homebrew/lib -lGLEW -lglfw -framework OpenGL
SRC_DIR = src
OBJ_DIR = obj
BIN = OpenGLApp

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean