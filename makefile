TARGET			= cabaret_tetrahedral
TEST_TARGET		= cabaret_tetrahedral_test
MAIN_CLASS		= Main.cpp

SRC_DIR			= src
INCLUDE_DIR		= include
LIB_DIR			= lib
BIN_DIR			= bin
BUILD_DIR		= build
TEST_DIR		= test
OUTPUT_DIR		= $(BIN_DIR)/output

CXX       		= g++
CXXFLAGS  		= -g -std=c++11 -Wall -Wextra

LINKER   		= g++ -o
LFLAGS   		= -Wall -Wextra -lconfig++

INCLUDES 		= $(wildcard $(INCLUDE_DIR)/*.hpp)

SOURCES  		= $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  		= $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
SOURCES_NO_MAIN	= $(filter-out $(SRC_DIR)/$(MAIN_CLASS), $(wildcard $(SRC_DIR)/*.cpp))
OBJECTS_NO_MAIN	= $(SOURCES_NO_MAIN:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_SOURCES	= $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS	= $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) -I $(INCLUDE_DIR) $(OBJECTS)
	@echo "Target linked successfully!"

$(OBJECTS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(BIN_DIR)/$(TEST_TARGET): $(TEST_OBJECTS) $(OBJECTS_NO_MAIN)
	@$(LINKER) $@ $(LFLAGS) -I $(INCLUDE_DIR) -I $(TEST_DIR)/catch.hpp $(TEST_OBJECTS) $(OBJECTS_NO_MAIN)
	@echo "Test target linked successfully!"

$(TEST_OBJECTS): $(BUILD_DIR)/%.o : $(TEST_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -c $< -o $@
	@echo "Compiled "$<" successfully!"	

target: $(BIN_DIR)/$(TARGET)

target_test: $(BIN_DIR)/$(TEST_TARGET)

run_target: target
	@mkdir -p $(OUTPUT_DIR)
	./$(BIN_DIR)/$(TARGET)

run_test: target_test
	./$(BIN_DIR)/$(TEST_TARGET)

clean_all:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned object files successfully!"
	@rm -f $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(TEST_TARGET)
	@echo "Cleaned bin files successfully!"
	@rm -rf $(OUTPUT_DIR)/
	@echo "Cleaned result files successfully!"

clean_result:
	@rm -rf $(OUTPUT_DIR)/
	@echo "Cleaned result files successfully!"