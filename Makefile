# all:
# 	g++ main.cpp game.cpp round.cpp player.cpp human.cpp deck.cpp card.cpp computer.cpp
# 	./a.out 
#

CXX = g++
CXX_FLAGS = -std=c++17
# SRCS = $(wildcard *.cpp)
CPP = main.cpp $(wildcard *.cpp)

ifeq "$(findstring debug, $(MAKECMDGOALS))" "debug"
	BIN = debug
	BUILD_DIR = ./build_debug
	CCCALLFLAGS += -g3 -O0 -D_DEBUG -D_GLIBCXX_DEBUG -Wall -Wno-unknown-pragmas -Wno-format
	CXX_FLAGS += -Wfatal-errors -Wall -Wextra -Wpedantic -Wconversion -Wshadow
else
	BIN = mybin
	BUILD_DIR = ./build_release
endif

# All .o files go to build dir.
OBJ = $(CPP:%.cpp=$(BUILD_DIR)/%.o)
# Gcc/Clang will create these .d files containing dependencies.
DEP = $(OBJ:%.o=%.d)

# Default target named after the binary.
$(BIN) : $(BUILD_DIR)/$(BIN)

# Actual target of the binary - depends on all .o files.
$(BUILD_DIR)/$(BIN) : $(OBJ)
	# Create build directories - same structure as sources.
	mkdir -p $(@D)
	# Just link all the object files.
	$(CXX) $(CXX_FLAGS) $^ -o $@

 # Include all .d files
 # These will be created every time
 -include $(DEP)

# The -MMD flags additionaly creates a .d file with
# the same name as the .o file.
$(BUILD_DIR)/%.o : %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

.PHONY : clean
clean :
	# This should remove all generated files.
	-rm -r ./build_release/ ./build_debug

# $(info $$BUILD_DIR is ${BUILD_DIR}])
# $(info $$OBJ is ${OBJ}])
# $(info $$DEP is ${DEP}])
