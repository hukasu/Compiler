#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	// Simple alternation with epsilon path
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test{
		compiler::NFATransition {0, 1, '\0'},
		compiler::NFATransition {1, 2, '\0'},
		compiler::NFATransition {2, 3, 'a'},
		compiler::NFATransition {3, 4, 'a'},
		compiler::NFATransition {4, 5, 'a'},
		compiler::NFATransition {5, 6, 'a'},
		compiler::NFATransition {6, 7, 'a'},
		compiler::NFATransition {1, 8, '\0'},
		compiler::NFATransition {8, 9, 'b'},
		compiler::NFATransition {9, 10, 'b'},
		compiler::NFATransition {10, 11, 'b'},
		compiler::NFATransition {11, 12, 'b'},
		compiler::NFATransition {12, 13, 'b'},
		compiler::NFATransition {1, 14, '\0'},
		compiler::NFATransition {7, 15, '\0'},
		compiler::NFATransition {13, 15, '\0'},
		compiler::NFATransition {14, 15, '\0'},
	};

	testSuccessfulRegistration(nfa, "{a:{aaaaa}{bbbbb}{}}");
	testEqualNFATransitions(nfa, nfa_test);

	return 0;
}