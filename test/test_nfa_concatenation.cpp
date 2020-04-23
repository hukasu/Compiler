#include <algorithm>
#include <iterator>
#include <nfa.hpp>

int main(int argc, char **argv) {
	compiler::NFA nfa;
	nfa.registerRegex("abc");
	std::set<compiler::NFATransition> nfa_transitions = nfa.getTransitions();
	std::set<compiler::NFATransition> nfa_test{
		compiler::NFATransition {
			0, 1, '\0'
		},
		compiler::NFATransition {
			1, 2, 'a'
		},
		compiler::NFATransition {
			2, 3, 'b'
		},
		compiler::NFATransition {
			3, 4, 'c'
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