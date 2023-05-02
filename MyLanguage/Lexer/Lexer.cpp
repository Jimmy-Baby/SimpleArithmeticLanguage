// Precompiled headers
#include "Pch.hpp"

#include "Lexer.hpp"
#include "Utility/Error.hpp"


void CLexer::Advance()
{
	m_Pos.Advance(m_CurrentChar);

	if (m_Pos.Index() < static_cast<i32>(m_Input.size()))
	{
		m_CurrentChar = m_Input[m_Pos.Index()];
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
		return CToken(TYPE_INT, numString);
	}

	return CToken(TYPE_FLOAT, numString);
}


CError CLexer::MakeTokens(std::vector<CToken>& tokensOut)
{
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
			tokensOut.emplace_back(CToken(TYPE_PLUS));
			Advance();
		}
		else if (ch == '-')
		{
			tokensOut.emplace_back(CToken(TYPE_MINUS));
			Advance();
		}
		else if (ch == '*')
		{
			tokensOut.emplace_back(CToken(TYPE_MUL));
			Advance();
		}
		else if (ch == '/')
		{
			tokensOut.emplace_back(CToken(TYPE_DIV));
			Advance();
		}
		else if (ch == '(')
		{
			tokensOut.emplace_back(CToken(TYPE_LBRACKET));
			Advance();
		}
		else if (ch == ')')
		{
			tokensOut.emplace_back(CToken(TYPE_RBRACKET));
			Advance();
		}
		else
		{
			const std::string errorStr = std::format("'{}'", m_CurrentChar);
			const CPosition errorStartPosition = m_Pos;

			// Advance so we can recapture m_Pos after it has moved forward
			Advance();

			return CError("Illegal Character", errorStr, errorStartPosition, m_Pos);
		}
	}

	return {};
}
