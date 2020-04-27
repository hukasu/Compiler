#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	// Simple alternation with epsilon path
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test;

	if (!testExpectException < compiler::NFA::UnexpectedEndOfStringException>(nfa, "{a:{abc}{def}}"))
		throw std::runtime_error("Registration didn't throw expected exception");
	if (testEqualNFATransitions(nfa, nfa_test))
		throw std::runtime_error("Clean up after raised exception failed");

	return 0;
}