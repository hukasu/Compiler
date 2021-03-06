#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	// Simple concatenation
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test{
		compiler::NFATransition {0, 1, '\0'},
		compiler::NFATransition {1, 2, 'a'},
		compiler::NFATransition {2, 3, 'b'},
		compiler::NFATransition {3, 4, 'c'},
	};

	testSuccessfulRegistration(nfa, "abc");
	testEqualNFATransitions(nfa, nfa_test);

	return 0;
}