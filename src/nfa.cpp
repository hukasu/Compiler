#include "nfa.hpp"

#include <queue>

namespace compiler {
	bool NFATransition::operator<(const NFATransition &_Right) const {
		if (m_source == _Right.m_source) {
			if (m_destination == _Right.m_destination) {
				return m_character < _Right.m_character;
			} else {
				return m_destination < _Right.m_destination;
			}
		} else {
			return m_source < _Right.m_source;
		}
	}

	bool NFATransition::operator==(const compiler::NFATransition &_Right) const {
		return
			m_source == _Right.m_source &&
			m_destination == _Right.m_destination &&
			m_character == _Right.m_character;
	}

	NFA::NFA() {
		m_nodes.push_back(
			NFANode{
				false,
				std::map<char, uint64_t>(),
				std::set<uint64_t>()
			}
		);
	}

	uint64_t NFA::addNode() {
		uint64_t new_id = m_nodes.size();
		m_nodes.push_back(
			NFANode{
				false,
				std::map<char, uint64_t>(),
				std::set<uint64_t>()
			}
		);
		return new_id;
	}

	void NFA::addTransition(uint64_t _src, uint64_t _dest, char _transition) {
		if (_transition == '\0') {
			m_nodes[_src].m_epsilon_transitions.insert(_dest);
		} else {
			m_nodes[_src].m_transitions.insert(
				std::make_pair(
					_transition,
					_dest
				)
			);
		}
	}

	bool NFA::retrieveChar(std::stringstream &_regex, char &_out_char) {
		_regex.get(_out_char);
		if (_regex.eof()) {
			return false;
		}
		return true;
	}

	NFA::NFARegexState NFA::registerCharacter(std::stringstream& _regex, uint64_t _current) {
		char c;
		bool read_char = retrieveChar(_regex, c);
		if (!read_char) {
			return NFARegexState{
				_current,
				NFARegexState::ReturnType::eEndOfString
			};
		}

		if (c == '{') {
			return beginOpenBracket(_regex, _current);
		} else if (c == '}') {
			return NFARegexState{
				_current,
				NFARegexState::ReturnType::eEndOfBracket
			};
		} else {
			// This is a single literal character
			// Simply add a transaction from the previous character to the new one
			uint64_t new_id = addNode();
			addTransition(_current, new_id, c);
			return registerCharacter(_regex, new_id);
		}
	}

	NFA::NFARegexState NFA::beginOpenBracket(std::stringstream &_regex, uint64_t _current) {
		char c;
		bool read_char = retrieveChar(_regex, c);
		if (!read_char) {
			throw UnexpectedEndOfStringException();
		}

		switch (c) {
		case 'a':
			return alternation(_regex, _current);
			break;
		case 'b':
			return beginOfString(_regex, _current);
			break;
		case 'e':
			return endOfString(_regex, _current);
			break;
		case 'l':
			return literalCharacter(_regex, _current);
			break;
		case 'r':
			return repetition(_regex, _current);
			break;
		default:
			throw UnknownBracketOperationException(c, static_cast<size_t>(_regex.tellg()) - 1);
		}
	}

	NFA::NFARegexState NFA::alternation(std::stringstream &_regex, uint64_t _current) {
		char c;
		bool read_char = retrieveChar(_regex, c);
		if (!read_char) {
			throw UnexpectedEndOfStringException();
		}
		if (c != ':') {
			throw ExpectedColonException(static_cast<size_t>(_regex.tellg()) - 1);
		}

		std::vector<uint64_t> path_ends;
		bool more_paths = true;
		while (more_paths) {
			NFARegexState rs = alternationPath(_regex, _current);
			switch (rs.m_return_type) {
			case NFARegexState::ReturnType::eAlternationPathEnd:
				path_ends.push_back(rs.m_last_id);
				break;
			case NFARegexState::ReturnType::eEndOfBracket:
				more_paths = false;
				break;
			case NFARegexState::ReturnType::eEndOfString:
				throw UnexpectedEndOfStringException();
			default:
				throw InternalErrorException();
			}
		}

		if (path_ends.size() < 2) {
			throw ShortAlternationException(static_cast<size_t>(_regex.tellg()) - 1);
		}

		uint64_t end_node = addNode();
		for (uint64_t path_end : path_ends) {
			addTransition(path_end, end_node, '\0');
		}
		return registerCharacter(_regex, end_node);
	}

	NFA::NFARegexState NFA::alternationPath(std::stringstream &_regex, uint64_t _current) {
		char c;
		bool read_char = retrieveChar(_regex, c);
		if (!read_char) {
			throw UnexpectedEndOfStringException();
		}
		if (c == '{') {
			uint64_t new_id = addNode();
			addTransition(_current, new_id, '\0');
			NFARegexState rs = registerCharacter(_regex, new_id);

			switch (rs.m_return_type) {
			case NFARegexState::ReturnType::eEndOfBracket:
				return NFARegexState{
					rs.m_last_id,
					NFARegexState::ReturnType::eAlternationPathEnd
				};
			case NFARegexState::ReturnType::eEndOfString:
				throw UnexpectedEndOfStringException();
			default:
				throw InternalErrorException();
			}
		} else if (c == '}') {
			return NFARegexState{
				_current,
				NFARegexState::ReturnType::eEndOfBracket
			};
		} else {
			char chars[2] = { c, '\0' }; // TODO make UTF-8
			throw UnexpectedCharacterInAlternationException(chars, static_cast<size_t>(_regex.tellg()) - std::strlen(chars));
		}
	}

	NFA::NFARegexState NFA::repetition(std::stringstream &_regex, uint64_t _current) {
		char c;
		bool read_char = retrieveChar(_regex, c);
		if (!read_char) {
			throw UnexpectedEndOfStringException();
		}
		
		switch (c) {
		case ';':
			// TODO fixed number of repetitions
			return NFARegexState{ 0, static_cast<NFA::NFARegexState::ReturnType>(-1) };
		
		case ':':
			uint64_t start_of_repetition;
			start_of_repetition = addNode();
			NFARegexState rs = registerCharacter(_regex, start_of_repetition);
			switch (rs.m_return_type) {
			case NFA::NFARegexState::ReturnType::eEndOfBracket:
				uint64_t end_of_repetition;
				end_of_repetition = addNode();
				addTransition(start_of_repetition, end_of_repetition, '\0');
				addTransition(rs.m_last_id, end_of_repetition, '\0');
				return registerCharacter(_regex, end_of_repetition);
			
			case NFA::NFARegexState::ReturnType::eEndOfString:
				throw UnexpectedEndOfStringException();
			
			default:
				throw InternalErrorException();
			}
		
		default:
			throw ExpectedColonException(static_cast<size_t>(_regex.tellg()) - 1);
		}
	}

	NFA::NFARegexState NFA::fixedLengthRepetition(std::stringstream &_regex, uint64_t _current) {
		int64_t length = 0;
		_regex >> length;
		
		if (_regex.gcount() > 0) {
			if (length <= 0) {
				throw std::runtime_error("Repetition can't have negative lenght");
			}

			char c;
			bool read_char = retrieveChar(_regex, c);
			if (!read_char) {
				throw UnexpectedEndOfStringException();
			}

			switch (c) {
			case ';':
				return rangeRepetition(_regex, _current, length);

			case ':':
				NFARegexState rs;
				rs = doFixedLeghtRepetition(_regex, _current, length);
				switch (rs.m_return_type) {
				case NFARegexState::ReturnType::eFixedRepetitionEnd:
					return registerCharacter(_regex, rs.m_last_id);

				case NFARegexState::ReturnType::eEndOfString:
					throw UnexpectedEndOfStringException();

				default:
					throw InternalErrorException();
				}

			default:
				throw ExpectedColonException(static_cast<size_t>(_regex.tellg()) - 1);
			}
		} else {
			throw std::runtime_error("NAN at repetition length at byte [n].");
		}
	}

	NFA::NFARegexState NFA::rangeRepetition(std::stringstream &_regex, uint64_t _current, int64_t _min_range) {
		int64_t max_range = 0;
		_regex >> max_range;
		if (_regex.gcount() > 0) {
			if (max_range <= 0) {
				throw std::runtime_error("Repetition can't have negative lenght");
			}
			if (max_range < _min_range) {
				throw std::runtime_error("Max range of repetition is smaller than min range.");
			}

			char c;
			bool read_char = retrieveChar(_regex, c);
			if (!read_char) {
				throw UnexpectedEndOfStringException();
			}

			switch (c) {
			case ':':
			{
				std::streampos pos = _regex.tellg();
				NFARegexState rs = doFixedLeghtRepetition(_regex, _current, _min_range);
				switch (rs.m_return_type) {
				case NFARegexState::ReturnType::eFixedRepetitionEnd:
				{
					uint64_t last_id = rs.m_last_id;
					for (int64_t i = 0; i < (max_range - _min_range); i++) {
						_regex.seekg(pos);
						rs = doOption(_regex, last_id);
						switch (rs.m_return_type) {
						case NFARegexState::ReturnType::eOptionEnd:
							last_id = rs.m_last_id;
							break;

						case NFARegexState::ReturnType::eEndOfString:
							throw UnexpectedEndOfStringException();

						default:
							throw InternalErrorException();
						}
					}
					return registerCharacter(_regex, last_id);
				}

				case NFARegexState::ReturnType::eEndOfString:
					throw UnexpectedEndOfStringException();

				default:
					throw InternalErrorException();
				}
			}

			default:
				throw ExpectedColonException(static_cast<size_t>(_regex.tellg()) - 1);
			}
		} else {
			throw std::runtime_error("NAN at repetition length at byte [n].");
		}
	}

	NFA::NFARegexState NFA::doFixedLeghtRepetition(
		std::stringstream &_regex,
		uint64_t _current,
		int64_t _length
	) {
		std::streampos pos = _regex.tellg();
		uint64_t last_id = _current;
		for (int64_t i = 0; i < _length; i++) {
			_regex.seekg(pos);
			NFARegexState rs;
			rs = registerCharacter(_regex, last_id);

			switch (rs.m_return_type) {
			case NFA::NFARegexState::ReturnType::eEndOfBracket:
				last_id = rs.m_last_id;
				break;

			case NFA::NFARegexState::ReturnType::eEndOfString:
				throw UnexpectedEndOfStringException();

			default:
				throw InternalErrorException();
			}
		}
		return NFARegexState{
			last_id,
			NFARegexState::ReturnType::eFixedRepetitionEnd
		};
	}

	NFA::NFARegexState NFA::doOption(std::stringstream &_regex, uint64_t _current) {
		NFARegexState rs = registerCharacter(_regex, _current);
		switch (rs.m_return_type) {
		case NFARegexState::ReturnType::eEndOfBracket:
			addTransition(_current, rs.m_last_id, '\0');
			return NFARegexState{
				rs.m_last_id,
				NFARegexState::ReturnType::eOptionEnd
			};
			
		case NFARegexState::ReturnType::eEndOfString:
			throw UnexpectedEndOfStringException();

		default:
			throw InternalErrorException();
		}
	}

	NFA::NFARegexState NFA::beginOfString(std::stringstream &_regex, uint64_t _current) {
		uint64_t new_id = addNode();
		addTransition(_current, new_id, 0x02);

		char c;
		bool read_char = retrieveChar(_regex, c);
		if (!read_char) {
			throw UnexpectedEndOfStringException();
		}
		if (c != '}') {
			throw ExpectedEndOfBracketException(static_cast<size_t>(_regex.tellg()) - 1);
		}
		return registerCharacter(_regex, new_id);
	}

	NFA::NFARegexState NFA::endOfString(std::stringstream &_regex, uint64_t _current) {
		uint64_t new_id = addNode();
		addTransition(_current, new_id, 0x03);

		char c;
		bool read_char = retrieveChar(_regex, c);
		if (!read_char) {
			throw UnexpectedEndOfStringException();
		}
		if (c != '}') {
			throw ExpectedEndOfBracketException(static_cast<size_t>(_regex.tellg()) - 1);
		}
		return registerCharacter(_regex, new_id);
	}

	NFA::NFARegexState NFA::literalCharacter(std::stringstream &_regex, uint64_t _current) {
		char c;
		bool read_char = retrieveChar(_regex, c);
		if (!read_char) {
			throw UnexpectedEndOfStringException();
		}
		if (c != ':') {
			throw ExpectedColonException(static_cast<size_t>(_regex.tellg()) - 1);
		}
		
		read_char = retrieveChar(_regex, c);
		if (!read_char) {
			throw UnexpectedEndOfStringException();
		}
		uint64_t new_id = addNode();
		addTransition(_current, new_id, c);
		uint8_t bytes_to_read;
		if ((c & 0b10000000) == 0) { // single byte character
			bytes_to_read = 0;
		} else if ((c & 0b11100000) == 0b11000000) { // 2-byte character
			bytes_to_read = 1;
		} else if ((c & 0b11110000) == 0b11100000) { // 3-byte character
			bytes_to_read = 2;
		} else if ((c & 0b11111000) == 0b11110000) { // 4-byte character
			bytes_to_read = 3;
		} else {
			throw NonUTF8CharacterException(static_cast<size_t>(_regex.tellg()) - 1);
		}
		uint64_t prev_id = new_id;
		for (uint8_t i = 0; i < bytes_to_read; i++) {
			read_char = retrieveChar(_regex, c);
			if (!read_char) {
				throw UnexpectedEndOfStringException();
			}
			new_id = addNode();
			addTransition(prev_id, new_id, c);
			prev_id = new_id;
		}
		return registerCharacter(_regex, prev_id);
	}

	void NFA::registerRegex(std::string _regex) {
		std::stringstream regex(_regex, std::ios::in | std::ios::binary);

		uint64_t new_id = addNode();
		addTransition(0, new_id, '\0');

		try {
			NFARegexState rs = registerCharacter(regex, new_id);
			if (rs.m_return_type == NFARegexState::ReturnType::eEndOfBracket) {
				throw UnexpectedEndOfBracketException(static_cast<size_t>(regex.tellg()) - 1);
			}
		} catch (...) {
			std::vector<NFANode>::iterator beg = m_nodes.begin(), end = m_nodes.end();
			std::advance(beg, new_id);
			m_nodes.erase(beg, end);

			m_nodes[0].m_epsilon_transitions.erase(new_id);

			std::rethrow_exception(std::current_exception());
		}
	}

	std::set<NFATransition> NFA::getTransitions() {
		std::set<NFATransition> transitions;
		for (size_t i = 0; i < m_nodes.size(); i++) {
			NFANode& node = m_nodes[i];
			for (uint64_t transition : node.m_epsilon_transitions) {
				transitions.insert(
					NFATransition{
						i,
						transition,
						'\0'
					}
				);
			}
			for (std::pair<char, uint64_t> transition : node.m_transitions) {
				transitions.insert(
					NFATransition{
						i,
						transition.second,
						transition.first
					}
				);
			}
		}
		return std::move(transitions);
	}

	std::string NFA::UnexpectedEndOfStringException::buildMessage() {
		return "Unexpected end of regular expression.";
	}

	NFA::UnexpectedEndOfStringException::UnexpectedEndOfStringException()
		: std::runtime_error(buildMessage()) {}

	std::string NFA::ExpectedColonException::buildMessage(size_t _byte_position) {
		std::stringstream ss;
		ss << "Expected ':' at byte [" << _byte_position << "].";
		throw std::runtime_error(ss.str());
	}

	NFA::ExpectedColonException::ExpectedColonException(size_t _byte_position)
		: std::runtime_error(buildMessage(_byte_position)) {}

	std::string NFA::ShortAlternationException::buildMessage(size_t _byte_position) {
		std::stringstream ss;
		ss << "Alternation at byte [" << _byte_position << "] requires at least 2 paths.";
		throw std::runtime_error(ss.str());
	}

	NFA::ShortAlternationException::ShortAlternationException(size_t _byte_position)
		: std::runtime_error(buildMessage(_byte_position)) {}

	std::string NFA::UnexpectedCharacterInAlternationException::buildMessage(char *_c, size_t _byte_position) {
		std::stringstream ss;
		ss << "Found '" << _c << "' outside of alternation brackets at byte [" << _byte_position << "].";
		throw std::runtime_error(ss.str());
	}

	NFA::UnexpectedCharacterInAlternationException::UnexpectedCharacterInAlternationException(char *_c, size_t _byte_position)
		: std::runtime_error(buildMessage(_c, _byte_position)) {}

	std::string NFA::ExpectedEndOfBracketException::buildMessage(size_t _byte_position) {
		std::stringstream ss;
		ss << "Expected '}' at byte [" << _byte_position << "].";
		throw std::runtime_error(ss.str());
	}

	NFA::ExpectedEndOfBracketException::ExpectedEndOfBracketException(size_t _byte_position)
		: std::runtime_error(buildMessage(_byte_position)) {}

	std::string  NFA::UnexpectedEndOfBracketException::buildMessage(size_t _byte_position) {
		std::stringstream ss;
		ss << "Unexpected '}' at byte [" << _byte_position << "].";
		throw std::runtime_error(ss.str());
	}

	NFA::UnexpectedEndOfBracketException::UnexpectedEndOfBracketException(size_t _byte_position)
		: std::runtime_error(buildMessage(_byte_position)) {}

	std::string NFA::UnknownBracketOperationException::buildMessage(char _op, size_t _byte_position) {
		std::stringstream ss;
		ss 
			<< "Unknown curly brackets operation '" 
			<< _op 
			<< "' at byte [" 
			<< _byte_position 
			<< "].";
		throw std::runtime_error(ss.str());
	}
	
	NFA::UnknownBracketOperationException::UnknownBracketOperationException(char _op, size_t _byte_position)
			: std::runtime_error(buildMessage(_op, _byte_position)) {}

	std::string NFA::NonUTF8CharacterException::buildMessage(size_t _byte_position) {
		std::stringstream ss;
		ss << "Found non UTF-8 character at byte [" << _byte_position << "].";
		throw std::runtime_error(ss.str());
		return "Couldn't process regular expression due to internal error.";
	}

	NFA::NonUTF8CharacterException::NonUTF8CharacterException(size_t _byte_position)
		: std::runtime_error(buildMessage(_byte_position)) {}

	std::string NFA::InternalErrorException::buildMessage() {
		return "Couldn't process regular expression due to internal error.";
	}

	NFA::InternalErrorException::InternalErrorException()
		: std::runtime_error(buildMessage()) {}
}