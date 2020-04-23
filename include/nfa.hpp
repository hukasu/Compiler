#ifndef __NFA__HPP__
#define __NFA__HPP__

#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>

namespace compiler {
	struct NFATransition {
		uint64_t m_source;
		uint64_t m_destination;
		char m_character;

		bool operator<(const NFATransition &_right) const;
		bool operator==(const NFATransition &_right) const;
	};

	class NFA {
		struct NFARegexState {
			uint64_t m_last_id;
			enum class ReturnType : uint8_t {
				eEndOfString,
				eEndOfBracket
			} m_return_type;
		};
		struct NFANode {
			bool m_final_state = false;
			std::multimap<char, uint64_t> m_transitions;
		};
		std::vector<NFANode> m_nodes;

		uint64_t addNode();
		void addTransition(uint64_t _src, uint64_t _dest, char _transition);

		bool retrieveChar(std::stringstream &_regex, char &_out_char);
		NFARegexState registerCharacter(std::stringstream &_regex, uint64_t _current);

		NFARegexState beginOpenBracket(std::stringstream &_regex, uint64_t _current);

		NFARegexState alternation(std::stringstream &_regex, uint64_t _current);
		NFARegexState alternationPath(std::stringstream &_regex, uint64_t _current);

		NFARegexState beginOfString(std::stringstream &_regex, uint64_t _current);
		NFARegexState endOfString(std::stringstream &_regex, uint64_t _current);

		NFARegexState literalCharacter(std::stringstream &_regex, uint64_t _current);
	public:
		NFA();
		~NFA() = default;

		void registerRegex(std::string _regex);

		/**
		* Note: due to how it's implemented, UTF-8 char will have each of it's bytes as a different transision
		*/
		std::set<NFATransition> getTransitions();
	public:
		class UnexpectedEndOfStringException : public std::runtime_error {
			static std::string buildMessage();
		public:
			UnexpectedEndOfStringException();
		};

		class ExpectedColonException : public std::runtime_error {
			static std::string buildMessage(size_t _byte_position);
		public:
			ExpectedColonException(size_t _byte_position);
		};

		class ShortAlternationException : public std::runtime_error {
			static std::string buildMessage(size_t _byte_position);
		public:
			ShortAlternationException(size_t _byte_position);
		};

		class UnexpectedCharacterInAlternationException : public std::runtime_error {
			static std::string buildMessage(char *_c, size_t _byte_position);
		public:
			UnexpectedCharacterInAlternationException(char *_c, size_t _byte_position);
		};

		class ExpectedEndOfBracketException : public std::runtime_error {
			static std::string buildMessage(size_t _byte_position);
		public:
			ExpectedEndOfBracketException(size_t _byte_position);
		};

		class UnexpectedEndOfBracketException : public std::runtime_error {
			static std::string buildMessage(size_t _byte_position);
		public:
			UnexpectedEndOfBracketException(size_t _byte_position);
		};

		class UnknownBracketOperationException : public std::runtime_error {
			static std::string buildMessage(char _op, size_t _byte_position);
		public:
			UnknownBracketOperationException(char _op, size_t _byte_position);
		};

		class NonUTF8CharacterException : public std::runtime_error {
			static std::string buildMessage(size_t _byte_position);
		public:
			NonUTF8CharacterException(size_t _byte_position);
		};

		class InternalErrorException : public std::runtime_error {
			static std::string buildMessage();
		public:
			InternalErrorException();
		};
	};
}

#endif // __NFA__HPP__