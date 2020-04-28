#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test{
		compiler::NFATransition {0, 1, '\0'},
		compiler::NFATransition {1, 2, 'a'},
		compiler::NFATransition {2, 3, 'b'},
		compiler::NFATransition {3, 4, 'c'},
	};

	testSuccessfulRegistration(nfa, "abc");
	testEqualNFATransitions(nfa, nfa_test);

	nfa_test.insert(compiler::NFATransition{ 0, 5, '\0' });
	nfa_test.insert(compiler::NFATransition{ 5, 6, 'a' });
	nfa_test.insert(compiler::NFATransition{ 6, 7, 'b' });
	nfa_test.insert(compiler::NFATransition{ 7, 8, 'c' });
	testSuccessfulRegistration(nfa, "abc");
	testEqualNFATransitions(nfa, nfa_test);

	return 0;
}