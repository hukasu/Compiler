#include <functional>

#include "common/test_nfa_common.hpp"

bool testSimpleConcatenation();
bool testSimpleAlternation();
bool testEpsilonAlternation();

int main(int argc, char **argv) {
	std::vector<std::function<bool()>> tests{
		testSimpleConcatenation,
		testSimpleAlternation,
		testEpsilonAlternation
	};
	for (size_t i = 0; i < tests.size(); i++) {
		if (!tests[i]()) return static_cast<int>(i) + 1;
	}
	return 0;
}

bool testSimpleConcatenation() {
	// Simple concatenation
	std::set<compiler::NFATransition> nfa_test{
		compiler::NFATransition {0, 1, '\0'},
		compiler::NFATransition {1, 2, 'a'},
		compiler::NFATransition {2, 3, 'b'},
		compiler::NFATransition {3, 4, 'c'},
	};
	return testSuccessfulRegistration("abc", nfa_test);
}

bool testSimpleAlternation() {
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
	return testSuccessfulRegistration("{a:{abc}{def}}", nfa_test);
}

bool testEpsilonAlternation() {
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
	return testSuccessfulRegistration("{a:{aaaaa}{bbbbb}{}}", nfa_test);
}

bool testIncompleteRegex() {
	return testExpectException<compiler::NFA::UnexpectedEndOfStringException>("{a:{abc}{def}");
}