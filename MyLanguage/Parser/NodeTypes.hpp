#pragma once

#include "Lexer/Token.hpp"

class CNumberNode
{
public:
	CNumberNode(CToken* token)
	: m_Token(token)
	{
	}

	void Print() const
	{
		m_Token->Print();
	}


private:
	CToken* m_Token;
};

class BinaryOpNode
{
public:	
	BinaryOpNode(CNumberNode left, CToken* token, CNumberNode right)
		: m_LeftNode(left),
		  m_Token(token),
		  m_RightNode(right)
	{

	}

private:
	CNumberNode m_LeftNode;
	CToken* m_Token;
	CNumberNode m_RightNode;
};