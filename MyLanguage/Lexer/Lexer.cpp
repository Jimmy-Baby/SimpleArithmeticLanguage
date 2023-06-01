// Precompiled headers
#include "Pch.hpp"

#include "Lexer.hpp"
#include "Utility/ErrorManager.h"

std::vector<CToken> CLexer::MakeTokens(std::string* input)
{
	m_Input = input;
	m_Pos = CPosition(-1, 0, -1, *m_Input);
	m_CurrentChar = '\0';

	Advance();

	std::vector<CToken> tokensOut;
	const char& ch = m_CurrentChar;

	while (ch != '\0')
	{
		if (ch == ' ' || ch == '\t')
		{
			Advance();
		}
		else if (isdigit(ch))
		{
			tokensOut.emplace_back(NumberToken());
		}
		else if (ch == '+')
		{
			tokensOut.emplace_back(CToken(TYPE_PLUS, "", m_Pos));
			Advance();
		}
		else if (ch == '-')
		{
			tokensOut.emplace_back(CToken(TYPE_MINUS, "", m_Pos));
			Advance();
		}
		else if (ch == '*')
		{
			tokensOut.emplace_back(CToken(TYPE_MUL, "", m_Pos));
			Advance();
		}
		else if (ch == '/')
		{
			tokensOut.emplace_back(CToken(TYPE_DIV, "", m_Pos));
			Advance();
		}
		else if (ch == '(')
		{
			tokensOut.emplace_back(CToken(TYPE_LBRACKET, "", m_Pos));
			Advance();
		}
		else if (ch == ')')
		{
			tokensOut.emplace_back(CToken(TYPE_RBRACKET, "", m_Pos));
			Advance();
		}
		else
		{
			const std::string errorStr = std::format("'{}'", m_CurrentChar);
			const CPosition errorStartPosition = m_Pos;

			// Advance so we can recapture m_Pos after it has moved forward
			Advance();

			g_ErrorMgr->Create<CError>("Illegal Character", errorStr, errorStartPosition, m_Pos);
			return {};
		}
	}

	tokensOut.emplace_back(CToken(TYPE_EOF, "", m_Pos));
	return tokensOut;
}

void CLexer::Advance()
{
	m_Pos.Advance(m_CurrentChar);

	if (m_Pos.Index() < static_cast<i32>(m_Input->size()))
	{
		m_CurrentChar = m_Input->at(m_Pos.Index());
	}
	else
	{
		m_CurrentChar = '\0';
	}
}

[[nodiscard]] CToken CLexer::NumberToken()
{
	std::string numString;
	i32 dotCount = 0;

	CPosition posStart = m_Pos;

	while (m_CurrentChar != '\0' && (isdigit(m_CurrentChar) || m_CurrentChar == '.'))
	{
		if (m_CurrentChar == '.')
		{
			if (dotCount == 1)
			{
				break;
			}

			dotCount++;
			numString += '.';
		}
		else
		{
			numString += m_CurrentChar;
		}

		Advance();
	}

	if (dotCount == 0)
	{
		return CToken(TYPE_INT, numString, std::move(posStart), m_Pos);
	}

	return CToken(TYPE_FLOAT, numString, std::move(posStart), m_Pos);
}
