# This file is called using 'include', so the working directory is the same as CMakeLists.txt
function (create_test test_subfolder test_name)
	add_executable(
		${test_name}
		"test/${test_subfolder}/${test_name}.cpp"
	)
	target_link_libraries(
		${test_name}
		PUBLIC CompilerLib
	)
	target_include_directories(
		${test_name}
		PUBLIC "test/common"
	)
	add_test(
		NAME ${test_name}
		COMMAND ${test_name}
	)
endfunction()

create_test(nfa test_nfa_simple_concatenation)
create_test(nfa test_nfa_simple_alternation)
create_test(nfa test_nfa_simple_alternation_with_epsilon)
create_test(nfa test_nfa_simple_option)
create_test(nfa test_nfa_simple_repetition)
create_test(nfa test_nfa_simple_fixed_length_repetition)
create_test(nfa test_nfa_short_alternation)
create_test(nfa test_nfa_incomplete_regex)
create_test(nfa test_nfa_no_colon)
create_test(nfa test_nfa_expect_end_bracket)
create_test(nfa test_nfa_multiple_regex)
create_test(nfa test_nfa_multiple_regex_with_failure)
