# This file is called using 'include', so the working directory is the same as CMakeLists.txt
add_executable(
	test_nfa
	"test/test_nfa.cpp"
)
target_link_libraries(
	test_nfa
	PUBLIC CompilerLib
)
add_test(
	NAME test_nfa
	COMMAND test_nfa
)