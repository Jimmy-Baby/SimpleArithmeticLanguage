#pragma once

#include "Token.hpp"
#include "Utility/Position.hpp"

class CLexer
{
public:
	static CLexer* GetInstance()
	{
		static CLexer lexer;
		return &lexer;
	}

	std::vector<CToken> MakeTokens(std::string* input);
	void Advance();
	[[nodiscard]] CToken NumberToken();

	// Protected fields and functions
protected:
	CLexer()
		: m_Input(),
		  m_Pos(CPosition(-1, 0, -1, "")),
		  m_CurrentChar('\0')
	{
	}

	std::string* m_Input;
	CPosition m_Pos;
	char m_CurrentChar;
};
