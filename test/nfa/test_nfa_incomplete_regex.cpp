#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	// Regex with unmatched brackets
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test;

	testExpectException<compiler::NFA::UnexpectedEndOfStringException>(nfa, "{a:{abc}{def}");
	testEqualNFATransitions(nfa, nfa_test);

	return 0;
}