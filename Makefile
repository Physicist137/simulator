GXX = g++ --std=c++14
INCLUDE = -Imodules

# Get paths for source and compiled objects of the tests.
# https://stackoverflow.com/questions/2483182/recursive-wildcards-in-gnu-make
TEST_SRC = $(shell find test/gtest_src/ -type f -name '*.cpp')
TEST_OBJ = $(patsubst test/gtest_src/%.cpp, test/gtest_obj/%.o, $(TEST_SRC))

# Get include and linkage of the gtests.
TEST_INCLUDE_PATH = "-Iexternal/googletest-release-1.11.0/googletest/include"
TEST_LINKAGE_PATH = "-Lexternal/googletest-release-1.11.0/googletest/build/lib"
TEST_MAIN_ARG = $(TEST_INCLUDE_PATH) $(TEST_LINKAGE_PATH) -lgtest  -lgtest_main  -pthread

# Define PHONY calls.
.PHONY: compile_tests display test clear

compile_tests: $(TEST_OBJ)

test: run_tests
	@./run_tests

test/gtest_obj/%.o: test/gtest_src/%.cpp
	@mkdir -p "$(@D)"
	@echo Compiling "$<"
	@$(GXX) -c "$<" -o "$@" $(INCLUDE) $(TEST_INCLUDE_PATH)

run_tests: test/main.cpp $(TEST_OBJ)
	@echo Compiling "$<"
	@$(GXX) $(TEST_OBJ) "$<" -o "$@" $(TEST_MAIN_ARG)

clear:
	@rm -rf test/gtest_obj
	@rm -f run_tests

display:
	@echo TEST_SRC=$(TEST_SRC)
	@echo TEST_OBJ=$(TEST_OBJ)