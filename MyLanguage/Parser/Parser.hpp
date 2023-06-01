#pragma once

#include "NodeTypes.hpp"

class CParser
{
	// Access functions
public:
	static CParser* GetInstance()
	{
		static CParser parser;
		return &parser;
	}

	CNodeBase* GetExpressionResult(std::vector<CToken>* tokens);
	CToken* Advance();
	[[nodiscard]] CNodeBase* GetFactor();
	[[nodiscard]] CNodeBase* GetTerm();
	[[nodiscard]] CNodeBase* GetExpression();

	template <class NodeTy, class... Args>
	[[nodiscard]] NodeTy* CreateNode(Args... nodeArgs)
	{
		// Create new node
		m_Nodes.emplace_back(std::make_unique<NodeTy>(nodeArgs...));

		// Get raw ptr to node and return
		return static_cast<NodeTy*>(m_Nodes.back().get());
	}

	template <class NodeTy, class RetTy, class... Args>
	[[nodiscard]] RetTy* CreateNode(Args... nodeArgs)
	{
		// Create new node
		m_Nodes.emplace_back(std::make_unique<NodeTy>(nodeArgs...));

		// Get raw ptr to node and return
		return static_cast<RetTy*>(m_Nodes.back().get());
	}

	// Protected fields and functions
protected:
	CParser()
		: m_CurrentToken(nullptr),
		  m_TokenIndex(-1),
		  m_Tokens(nullptr)
	{
	}

	std::vector<CToken>* m_Tokens;
	std::vector<std::unique_ptr<CNodeBase>> m_Nodes;
	CToken* m_CurrentToken;
	i32 m_TokenIndex;
};
