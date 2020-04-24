# This file is called using 'include', so the working directory is the same as CMakeLists.txt
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