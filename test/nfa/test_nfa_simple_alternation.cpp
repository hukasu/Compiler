#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	// Simple alternation
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test{
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

	if (!testSuccessfulRegistration(nfa, "{a:{abc}{def}}")) throw std::runtime_error("Failed to register regex");
	if (!testEqualNFATransitions(nfa, nfa_test)) throw std::runtime_error("Transition set were not equals");

	return 0;
}