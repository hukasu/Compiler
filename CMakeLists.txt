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
		test_concatenation
		"test/test_nfa_concatenation.cpp"
	)
	target_link_libraries(
		test_concatenation
		PUBLIC CompilerLib
	)
	add_test(
		NAME test_concatenation
		COMMAND test_concatenation
	)
endif()