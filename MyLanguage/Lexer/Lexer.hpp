#pragma once

#include "Token.hpp"
#include "Utility/Error.hpp"

class CLexer
{
public:
	explicit CLexer(std::string input, std::string fileName = "")
		: m_FileName(std::move(fileName)),
		  m_Input(std::move(input)),
		  m_Pos(CPosition(-1, 0, -1, m_FileName, m_Input)),
		  m_CurrentChar('\0')
	{
		Advance();
	}


	void Advance();
	[[nodiscard]] CToken NumberToken();
	CError MakeTokens(std::vector<CToken>& tokensOut);


private:
	std::string m_FileName;
	std::string m_Input;
	CPosition m_Pos;
	char m_CurrentChar;
};
