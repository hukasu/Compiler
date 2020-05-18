#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	// Fixed length bracket operations with extra characters
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test;

	// Begin of string does not accept any other character inside the brackets
	testExpectException<compiler::NFA::ExpectedEndOfBracketException>(nfa, "{ba}");
	testEqualNFATransitions(nfa, nfa_test);

	// End of string does not accept any other character inside the brackets
	testExpectException<compiler::NFA::ExpectedEndOfBracketException>(nfa, "{ea}");
	testEqualNFATransitions(nfa, nfa_test);

	return 0;
}