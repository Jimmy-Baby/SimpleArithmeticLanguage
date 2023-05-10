#pragma once

#include "Lexer/Token.hpp"

enum ENodeType
{
	NODE_TYPE_NUMBER,
	NODE_TYPE_BINARY_OP,
	NODE_TYPE_UNARY_OP
};

class CNodeBase : public CPrintable
{
public:
	explicit CNodeBase(const ENodeType type, CPosition start, CPosition end)
		: m_Type(type),
		  m_Start(std::move(start)),
		  m_End(std::move(end))
	{
	}


	[[nodiscard]] ENodeType GetType() const
	{
		return m_Type;
	}


	[[nodiscard]] CPosition GetStart() const
	{
		return m_Start;
	}


	[[nodiscard]] CPosition GetEnd() const
	{
		return m_End;
	}


	[[nodiscard]] virtual bool IsValid() const = 0;
	[[nodiscard]] virtual std::string GetPrintableTokenString() const = 0;

	// Protected fields and functions
protected:
	ENodeType m_Type;
	CPosition m_Start;
	CPosition m_End;
};

class CNumberNode final : public CNodeBase
{
public:
	explicit CNumberNode(CToken* token)
		: CNodeBase(NODE_TYPE_NUMBER, token->Start(), token->End()),
		  m_Token(token)
	{
	}


	[[nodiscard]] virtual bool IsValid() const override
	{
		return m_Token != nullptr;
	}


	[[nodiscard]] virtual std::string GetPrintableTokenString() const override
	{
		if (m_Token == nullptr)
		{
			return {};
		}

		return m_Token->GetPrintableTokenString();
	}


	virtual void Print() override
	{
		MyLang::Print(GetPrintableTokenString());
	}


	[[nodiscard]] CToken* GetToken() const
	{
		return m_Token;
	}


	[[nodiscard]] std::string& GetValue() const
	{
		return m_Token->GetValue();
	}


	[[nodiscard]] i32 GetValueNumber() const
	{
		return std::strtol(GetValue().c_str(), nullptr, 10);
	}

	// Protected fields and functions
protected:
	CToken* m_Token;
};

class CBinaryOpNode final : public CNodeBase
{
public:
	explicit CBinaryOpNode(CToken* token, CNodeBase* left, CNodeBase* right)
		: CNodeBase(NODE_TYPE_BINARY_OP, left->GetStart(), right->GetEnd()),
		  m_Token(token),
		  m_LeftNode(left),
		  m_RightNode(right)
	{
	}


	[[nodiscard]] virtual bool IsValid() const override
	{
		if (m_Token != nullptr &&
			m_LeftNode->IsValid() &&
			m_RightNode->IsValid())
		{
			return true;
		}

		return false;
	}


	[[nodiscard]] virtual std::string GetPrintableTokenString() const override
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


	virtual void Print() override
	{
		MyLang::Print(GetPrintableTokenString());
	}


	[[nodiscard]] CToken* GetToken() const
	{
		return m_Token;
	}


	[[nodiscard]] CNodeBase* GetLeftNode() const
	{
		return m_LeftNode;
	}


	[[nodiscard]] CNodeBase* GetRightNode() const
	{
		return m_RightNode;
	}

	// Protected fields and functions
protected:
	CToken* m_Token;
	CNodeBase* m_LeftNode;
	CNodeBase* m_RightNode;
};

class CUnaryOpNode final : public CNodeBase
{
public:
	explicit CUnaryOpNode(CToken* operatorToken, CNodeBase* node)
		: CNodeBase(NODE_TYPE_UNARY_OP, operatorToken->Start(), node->GetEnd()),
		  m_OpToken(operatorToken),
		  m_Node(node)
	{
	}


	[[nodiscard]] virtual bool IsValid() const override
	{
		return m_OpToken != nullptr && m_Node != nullptr;
	}


	[[nodiscard]] virtual std::string GetPrintableTokenString() const override
	{
		std::string token;
		std::string node;

		if (m_OpToken)
		{
			token = m_OpToken->GetPrintableTokenString();
		}

		if (m_Node)
		{
			node = m_Node->GetPrintableTokenString();
		}

		return std::format("({}, {})", token.c_str(), node.c_str());
	}


	virtual void Print() override
	{
		MyLang::Print(GetPrintableTokenString());
	}


	[[nodiscard]] CNodeBase* GetChildNode() const
	{
		return m_Node;
	}


	[[nodiscard]] CToken* GetToken() const
	{
		return m_OpToken;
	}

	// Protected fields and functions
protected:
	CToken* m_OpToken;
	CNodeBase* m_Node;
};
