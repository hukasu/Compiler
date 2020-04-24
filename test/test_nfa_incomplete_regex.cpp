#include <algorithm>
#include <iterator>
#include <nfa.hpp>

int main(int argc, char **argv) {
	compiler::NFA nfa;
	try {
		nfa.registerRegex("{a:{abc}{def}");
	} catch ([[maybe_unused]] compiler::NFA::UnexpectedEndOfStringException &e) {
		// Do nothing
	} catch (...) {
		return -1;
	}
	std::set<compiler::NFATransition> nfa_transitions = nfa.getTransitions();
	
	return static_cast<int>(nfa_transitions.size());
}