add_executable(
	test_nfa_incomplete_regex
	"test/test_nfa_incomplete_regex.cpp"
)
target_link_libraries(
	test_nfa_incomplete_regex
	PUBLIC CompilerLib
)
add_test(
	NAME test_nfa_incomplete_regex
	COMMAND test_nfa_incomplete_regex
)