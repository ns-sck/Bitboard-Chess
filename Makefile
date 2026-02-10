CXX      = g++
CXXFLAGS = -std=c++17 -Wall -I. -I./include/pieces
OBJ_DIR  = obj
TARGET   = game

SRCS     = $(wildcard *.cpp) $(wildcard src/pieces/*.cpp)

OBJS     = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

VPATH    = src/pieces

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

clean:
	if exist $(OBJ_DIR) rmdir /S /Q $(OBJ_DIR)
	if exist $(TARGET).exe del $(TARGET).exe