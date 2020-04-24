# This file is called using 'include', so the working directory is the same as CMakeLists.txt
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