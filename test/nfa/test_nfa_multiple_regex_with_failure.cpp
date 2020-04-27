#include <test_nfa_common.hpp>

int main(int argc, char **argv) {
	compiler::NFA nfa;
	std::set<compiler::NFATransition> nfa_test{
		compiler::NFATransition {0, 1, '\0'},
		compiler::NFATransition {1, 2, 'a'},
		compiler::NFATransition {2, 3, 'b'},
		compiler::NFATransition {3, 4, 'c'},
	};

	if (!testSuccessfulRegistration(nfa, "abc")) throw std::runtime_error("Failed to register regex");
	if (!testEqualNFATransitions(nfa, nfa_test)) throw std::runtime_error("Transition set were not equals");

	nfa_test.insert(compiler::NFATransition{ 0, 5, '\0' });
	nfa_test.insert(compiler::NFATransition{ 5, 6, 'a' });
	nfa_test.insert(compiler::NFATransition{ 6, 7, 'b' });
	nfa_test.insert(compiler::NFATransition{ 7, 8, 'c' });
	if (!testSuccessfulRegistration(nfa, "abc")) throw std::runtime_error("Failed to register regex");
	if (!testEqualNFATransitions(nfa, nfa_test)) throw std::runtime_error("Transition set were not equals");

	if (!testExpectException<compiler::NFA::UnexpectedEndOfStringException>(nfa, "abc{"))
		throw std::runtime_error("Registration didn't throw expected exception");
	if (!testEqualNFATransitions(nfa, nfa_test)) throw std::runtime_error("Transition set were not equals");

	return 0;
}