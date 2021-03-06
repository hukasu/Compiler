#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	// No colon on bracket operation
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test;

	testExpectException<compiler::NFA::ExpectedColonException>(nfa, "{a{abc}{def}}");
	testEqualNFATransitions(nfa, nfa_test);

	testExpectException<compiler::NFA::ExpectedColonException>(nfa, "{la}");
	testEqualNFATransitions(nfa, nfa_test);

	return 0;
}