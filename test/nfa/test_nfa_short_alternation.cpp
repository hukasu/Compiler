#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	// Simple alternation with epsilon path
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test;

	testExpectException<compiler::NFA::ShortAlternationException>(nfa, "{a:}");
	testEqualNFATransitions(nfa, nfa_test);

	testExpectException<compiler::NFA::ShortAlternationException>(nfa, "{a:{abc}}");
	testEqualNFATransitions(nfa, nfa_test);

	nfa_test = std::set<compiler::NFATransition>{
		compiler::NFATransition {0, 1, '\0'},
		compiler::NFATransition {1, 2, '\0'},
		compiler::NFATransition {2, 3, 'a'},
		compiler::NFATransition {3, 4, 'b'},
		compiler::NFATransition {4, 5, 'c'},
		compiler::NFATransition {5, 10, '\0'},
		compiler::NFATransition {1, 6, '\0'},
		compiler::NFATransition {6, 7, 'd'},
		compiler::NFATransition {7, 8, 'e'},
		compiler::NFATransition {8, 9, 'f'},
		compiler::NFATransition {9, 10, '\0'},
	};
	testSuccessfulRegistration(nfa, "{a:{abc}{def}}");
	testEqualNFATransitions(nfa, nfa_test);

	return 0;
}