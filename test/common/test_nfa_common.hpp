#ifndef __TEST__NFA__COMMON__HPP__
#define __TEST__NFA__COMMON__HPP__

#include <string>
#include <algorithm>
#include <iterator>
#include <set>
#include <iostream>

#include <nfa.hpp>

void testSuccessfulRegistration(
	compiler::NFA &_nfa,
	std::string _regex
) {
	_nfa.registerRegex(_regex);
}

template <typename T>
void testExpectException(
	compiler::NFA &_nfa,
	std::string _regex
) {
	static_assert(std::is_base_of<std::runtime_error, T>::value);
	try {
		_nfa.registerRegex(_regex);
		throw std::runtime_error("Didn't raise an exception");
	} catch (T &e) {
		std::cout << e.what() << std::endl;
		std::cout << "Exception correctly raised" << std::endl;
	}
}

void testEqualNFATransitions(
	compiler::NFA &_nfa,
	std::set<compiler::NFATransition> &_transitions
) {
	std::set<compiler::NFATransition> nfa_transitions = _nfa.getTransitions();
	std::set<compiler::NFATransition> res;
	std::set_symmetric_difference(
		nfa_transitions.begin(),
		nfa_transitions.end(),
		_transitions.begin(),
		_transitions.end(),
		std::inserter(res, res.begin())
	);
	if (res.size() != 0) throw std::runtime_error("NFA Transitions were not equal");
}

#endif // __TEST__NFA__COMMON__HPP__