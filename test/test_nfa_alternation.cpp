#include <algorithm>
#include <iterator>
#include <nfa.hpp>

int main(int argc, char **argv) {
	compiler::NFA nfa;
	nfa.registerRegex("{a:{abc}{def}}");
	std::set<compiler::NFATransition> nfa_transitions = nfa.getTransitions();
	std::set<compiler::NFATransition> nfa_test{
		compiler::NFATransition {
			0, 1, '\0'
		},
		compiler::NFATransition {
			1, 2, '\0'
		},
		compiler::NFATransition {
			2, 3, 'a'
		},
		compiler::NFATransition {
			3, 4, 'b'
		},
		compiler::NFATransition {
			4, 5, 'c'
		},
		compiler::NFATransition {
			5, 10, '\0'
		},
		compiler::NFATransition {
			1, 6, '\0'
		},
		compiler::NFATransition {
			6, 7, 'd'
		},
		compiler::NFATransition {
			7, 8, 'e'
		},
		compiler::NFATransition {
			8, 9, 'f'
		},
		compiler::NFATransition {
			9, 10, '\0'
		},
	};
	std::set<compiler::NFATransition> res;
	std::set_symmetric_difference(
		nfa_transitions.begin(),
		nfa_transitions.end(),
		nfa_test.begin(),
		nfa_test.end(),
		std::inserter(res, res.begin())
	);
	return static_cast<int>(res.size());
}