#ifndef __TEST__NFA__COMMON__HPP__
#define __TEST__NFA__COMMON__HPP__

#include <nfa.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>
#include <iostream>

#include <nfa.hpp>

bool testSuccessfulRegistration(
	compiler::NFA &_nfa,
	std::string _regex
) {
	try {
		_nfa.registerRegex(_regex);
		return true;
	} catch (std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return false;
	} catch (...) {
		std::cerr << "Unknown exception." << std::endl;
		return false;
	}
}

template <typename T>
bool testExpectException(
	compiler::NFA &_nfa,
	std::string _regex
) {
	static_assert(std::is_base_of<std::runtime_error, T>::value);
	try {
		try {
			_nfa.registerRegex(_regex);
			return false;
		} catch (T &e) {
			std::cout << e.what() << std::endl;
			std::cout << "Exception correctly raised" << std::endl;
			return true;
		}
	} catch (std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return false;
	} catch (...) {
		std::cerr << "Unknown exception." << std::endl;
		return false;
	}
}

bool testEqualNFATransitions(
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
	return res.size() == 0;
}

#endif // __TEST__NFA__COMMON__HPP__