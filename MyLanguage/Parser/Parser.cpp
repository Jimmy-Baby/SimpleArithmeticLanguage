// Precompiled headers
#include "Pch.hpp"

#include "Parser.hpp"

/*
 * expr: term ((PLUS | MINUS) term)*
 * term: factor ((MUL | DIV) factor)*
 * factor: INT | FLOAT
 *		   (PLUS|MINUS) factor
 *		   LBRACKET expr RBRACKET
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

[[nodiscard]] CNodeBase* CParser::GetFactor()
{
	MARK_FUNCTION_ERROR_MANAGEMENT;

	CToken* currentToken = m_CurrentToken;

	if (currentToken->Type() == TYPE_PLUS ||
		currentToken->Type() == TYPE_MINUS)
	{
		Advance();
		CNodeBase* factor = GetFactor();

		if (!g_ErrorMgr->CheckLastError(false, false, true))
		{
			return nullptr;
		}

		return CreateNode<CUnaryOpNode>(currentToken, factor);
	}

	if (currentToken->Type() == TYPE_INT ||
		currentToken->Type() == TYPE_FLOAT)
	{
		Advance();
		return CreateNode<CNumberNode>(currentToken);
	}

	if (currentToken->Type() == TYPE_LBRACKET)
	{
		Advance();
		CNodeBase* expression = GetExpression();

		if (!g_ErrorMgr->CheckLastError(false, false, true))
		{
			return nullptr;
		}

		if (m_CurrentToken->Type() == TYPE_RBRACKET)
		{
			Advance();
			return expression;
		}

		g_ErrorMgr->Create<CError>("Invalid Syntax", "Expected ')'", m_CurrentToken->Start(), m_CurrentToken->End());
		return nullptr;
	}

	g_ErrorMgr->Create<CError>("Invalid Syntax", "Expected integer or floating-point number", currentToken->Start(), currentToken->End());
	return nullptr;
}

[[nodiscard]] CNodeBase* CParser::GetTerm()
{
	MARK_FUNCTION_ERROR_MANAGEMENT;

	auto leftToken = GetFactor();

	if (!g_ErrorMgr->CheckLastError(false, false, true))
	{
		return nullptr;
	}

	while (m_CurrentToken->Type() == TYPE_MUL ||
		m_CurrentToken->Type() == TYPE_DIV)
	{
		CToken* operatorToken = m_CurrentToken;

		Advance();

		const auto rightToken = GetFactor();

		if (!g_ErrorMgr->CheckLastError(false, false, true))
		{
			return nullptr;
		}

		leftToken = static_cast<CNodeBase*>(CreateNode<CBinaryOpNode>(operatorToken, leftToken, rightToken));
	}

	return leftToken;
}

[[nodiscard]] CNodeBase* CParser::GetExpression()
{
	MARK_FUNCTION_ERROR_MANAGEMENT;

	auto leftToken = GetTerm();

	if (!g_ErrorMgr->CheckLastError(false, false, true))
	{
		return nullptr;
	}

	while (m_CurrentToken->Type() == TYPE_PLUS ||
		m_CurrentToken->Type() == TYPE_MINUS)
	{
		CToken* operatorToken = m_CurrentToken;

		Advance();

		const auto rightToken = GetTerm();

		if (!g_ErrorMgr->CheckLastError(false, false, true))
		{
			return nullptr;
		}

		leftToken = static_cast<CNodeBase*>(CreateNode<CBinaryOpNode>(operatorToken, leftToken, rightToken));
	}

	return leftToken;
}
