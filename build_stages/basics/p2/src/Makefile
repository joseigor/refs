# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Directories
SRC_DIR = .
MATH_DIR = math

# Source files
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/calculator.cpp \
       $(MATH_DIR)/add.cpp \
       $(MATH_DIR)/sub.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = main.out

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

# Compiling source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean