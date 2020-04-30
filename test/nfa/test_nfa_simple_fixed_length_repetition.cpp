#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	// Simple fixed length repetition
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test{
		compiler::NFATransition {0, 1, '\0'},
		compiler::NFATransition {1, 2, 'a'},
		compiler::NFATransition {2, 3, 'b'},
		compiler::NFATransition {3, 4, 'c'},
		compiler::NFATransition {4, 5, 'a'},
		compiler::NFATransition {5, 6, 'b'},
		compiler::NFATransition {6, 7, 'c'},
		compiler::NFATransition {7, 8, 'a'},
		compiler::NFATransition {8, 9, 'b'},
		compiler::NFATransition {9, 10, 'c'},
	};

	testSuccessfulRegistration(nfa, "{r;3:abc}");
	testEqualNFATransitions(nfa, nfa_test);

	return 0;
}