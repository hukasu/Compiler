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
	std::string _regex,
	std::set<compiler::NFATransition> &_transitions
) {
	try {
		compiler::NFA nfa;
		nfa.registerRegex(_regex);
		std::set<compiler::NFATransition> nfa_transitions = nfa.getTransitions();
		std::set<compiler::NFATransition> res;
		std::set_symmetric_difference(
			nfa_transitions.begin(),
			nfa_transitions.end(),
			_transitions.begin(),
			_transitions.end(),
			std::inserter(res, res.begin())
		);
		return res.size() == 0;
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
	std::string _regex
) {
	static_assert(std::is_base_of<std::runtime_error, T>::value);
	try {
		compiler::NFA nfa;
		try {
			nfa.registerRegex(_regex);
		} catch (
			T &e
		) {
			std::cout << e.what() << std::endl;
			std::cout << "Exception correctly raised" << std::endl;
		}
		std::set<compiler::NFATransition> nfa_transitions = nfa.getTransitions();
		return nfa_transitions.size() == 0;
	} catch (std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return false;
	} catch (...) {
		std::cerr << "Unknown exception." << std::endl;
		return false;
	}
}

#endif // __TEST__NFA__COMMON__HPP__