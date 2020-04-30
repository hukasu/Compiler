#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	// Simple repetition
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test{
		compiler::NFATransition {0, 1, '\0'},
		compiler::NFATransition {1, 6, '\0'},
		compiler::NFATransition {2, 3, 'a'},
		compiler::NFATransition {3, 4, 'b'},
		compiler::NFATransition {4, 5, 'c'},
		compiler::NFATransition {5, 6, '\0'},
		compiler::NFATransition {6, 2, '\0'},
	};

	testSuccessfulRegistration(nfa, "{r:abc}");
	testEqualNFATransitions(nfa, nfa_test);

	return 0;
}