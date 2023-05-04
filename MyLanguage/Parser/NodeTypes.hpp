﻿#pragma once

#include "Lexer/Token.hpp"

enum ENodeType
{
	NODE_TYPE_NUMBER,
	NODE_TYPE_BINARY_OP
};

class CNodeBase : public CPrintable
{
public:
	explicit CNodeBase(const ENodeType type)
		: m_Type(type)
	{
	}


	[[nodiscard]] ENodeType GetType() const
	{
		return m_Type;
	}


	[[nodiscard]] virtual bool IsValid() const = 0;
	[[nodiscard]] virtual std::string GetPrintableTokenString() const = 0;

private:
	ENodeType m_Type;
};

class CNumberNode : public CNodeBase
{
public:
	explicit CNumberNode(CToken* token)
		: CNodeBase(NODE_TYPE_NUMBER),
		  m_Token(token)
	{
	}


	[[nodiscard]] bool IsValid() const override
	{
		return m_Token != nullptr;
	}


	[[nodiscard]] std::string GetPrintableTokenString() const override
	{
		return m_Token->GetPrintableTokenString();
	}


	void Print() override
	{
		MyLang::Print(GetPrintableTokenString());
	}


private:
	CToken* m_Token;
};

class CBinaryOpNode : public CNodeBase
{
public:
	explicit CBinaryOpNode(CToken* token, CNodeBase* left, CNodeBase* right)
		: CNodeBase(NODE_TYPE_BINARY_OP),
		  m_Token(token),
		  m_LeftNode(left),
		  m_RightNode(right)
	{
	}


	[[nodiscard]] bool IsValid() const override
	{
		if (m_Token != nullptr &&
			m_LeftNode->IsValid() &&
			m_RightNode->IsValid())
		{
			return true;
		}

		return false;
	}


	[[nodiscard]] std::string GetPrintableTokenString() const override
	{
		std::string left;
		std::string token;
		std::string right;

		if (m_LeftNode)
		{
			left = m_LeftNode->GetPrintableTokenString();
		}

		if (m_Token)
		{
			token = m_Token->GetPrintableTokenString();
		}

		if (m_RightNode)
		{
			right = m_RightNode->GetPrintableTokenString();
		}

		return std::format("({}, {}, {})", left.c_str(), token.c_str(), right.c_str());
	}


	void Print() override
	{
		MyLang::Print(GetPrintableTokenString());
	}


private:
	CToken* m_Token;
	CNodeBase* m_LeftNode;
	CNodeBase* m_RightNode;
};
