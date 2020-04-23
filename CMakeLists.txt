cmake_minimum_required(VERSION 3.8.2)

project(Compiler)

option(BUILD_TESTS "Should build tests?" OFF)

add_library(
	CompilerLib
	STATIC
	src/nfa.cpp
 "test/test_nfa_concatenation.cpp")
target_include_directories(
	CompilerLib
	PUBLIC include/
)
target_compile_features(
	CompilerLib
	PUBLIC cxx_std_17
)

if (BUILD_TESTS)
	enable_testing()

	add_executable(
		test_nfa_concatenation
		"test/test_nfa_concatenation.cpp"
	)
	target_link_libraries(
		test_nfa_concatenation
		PUBLIC CompilerLib
	)
	add_test(
		NAME test_nfa_concatenation
		COMMAND test_nfa_concatenation
	)

	add_executable(
		test_nfa_alternation
		"test/test_nfa_alternation.cpp"
	)
	target_link_libraries(
		test_nfa_alternation
		PUBLIC CompilerLib
	)
	add_test(
		NAME test_nfa_alternation
		COMMAND test_nfa_alternation
	)
endif()