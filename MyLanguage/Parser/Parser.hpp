#pragma once

#include "NodeTypes.hpp"

class CParser
{
	// Access functions
public:
	explicit CParser(std::vector<CToken> tokens)
		: m_Tokens(std::move(tokens)),
		  m_CurrentToken(nullptr),
		  m_TokenIndex(-1)
	{
		Advance();
	}


	CBinaryOpNode* Run()
	{
		return GetExpression();
	}


	CToken* Advance();


	template <class NodeTy, class ...Args>
	[[nodiscard]] NodeTy* CreateNode(Args ...nodeArgs)
	{
		// Create new node
		m_Nodes.emplace_back(std::make_unique<NodeTy>(nodeArgs...));

		// Get raw ptr to node and return
		return dynamic_cast<NodeTy*>(m_Nodes.back().get());
	}


	[[nodiscard]] CNumberNode* GetFactor();
	[[nodiscard]] CBinaryOpNode* GetTerm();
	[[nodiscard]] CBinaryOpNode* GetExpression();


	// Protected functions
protected:
	// Member variables
private:
	std::vector<CToken> m_Tokens;
	std::vector<std::unique_ptr<CNodeBase>> m_Nodes;
	CToken* m_CurrentToken;
	i32 m_TokenIndex;
};
