#pragma once

#include "NodeTypes.hpp"

class CParser
{
	// Access functions
public:
	explicit CParser(const std::vector<CToken>& tokens)
		: m_Tokens(tokens)
	{
		Advance();
	}

	void Advance()
	{

	}

	// Protected functions
protected:
	// Member variables
private:
	const std::vector<CToken>& m_Tokens;
	i32 m_TokenIndex;
};
