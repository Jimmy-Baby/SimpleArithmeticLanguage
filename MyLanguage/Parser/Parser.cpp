// Precompiled headers
#include "Pch.hpp"

#include "Parser.hpp"

/*
 * expr: term ((PLUS | MINUS) term)*
 * term: factor ((MUL | DIV) factor)*
 * factor: INT | FLOAT
 */

CToken* CParser::Advance()
{
	m_TokenIndex++;

	if (m_TokenIndex < static_cast<i32>(m_Tokens.size()))
	{
		m_CurrentToken = &m_Tokens[m_TokenIndex];
	}

	return m_CurrentToken;
}


[[nodiscard]] CNumberNode* CParser::GetFactor()
{
	CToken* currentToken = m_CurrentToken;

	if (currentToken->Type() == TYPE_INT ||
		currentToken->Type() == TYPE_FLOAT)
	{
		Advance();
		return CreateNode<CNumberNode>(currentToken);
	}

	return CreateNode<CNumberNode>(nullptr);
}


[[nodiscard]] CBinaryOpNode* CParser::GetTerm()
{
	auto leftToken = static_cast<CNodeBase*>(GetFactor());

	while (m_CurrentToken->Type() == TYPE_MUL || m_CurrentToken->Type() == TYPE_DIV)
	{
		CToken* operatorToken = m_CurrentToken;

		Advance();

		const auto rightToken = static_cast<CNodeBase*>(GetFactor());
		leftToken = static_cast<CNodeBase*>(CreateNode<CBinaryOpNode>(operatorToken, leftToken, rightToken));
	}

	return static_cast<CBinaryOpNode*>(leftToken);
}


[[nodiscard]] CBinaryOpNode* CParser::GetExpression()
{
	auto leftToken = static_cast<CNodeBase*>(GetTerm());

	while (m_CurrentToken->Type() == TYPE_PLUS || m_CurrentToken->Type() == TYPE_MINUS)
	{
		CToken* operatorToken = m_CurrentToken;

		Advance();

		const auto rightToken = static_cast<CNodeBase*>(GetTerm());
		leftToken = static_cast<CNodeBase*>(CreateNode<CBinaryOpNode>(operatorToken, leftToken, rightToken));
	}

	return static_cast<CBinaryOpNode*>(leftToken);
}
