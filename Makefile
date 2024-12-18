CXX = g++
CXXFLAGS = -g -Wall -std=c++23
TARGET = kompress
OBJ_DIR = bin
SRC = main.cpp
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJ_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: %.cpp $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	rm -rf $(TARGET) $(OBJ_DIR)