#pragma once

#include "NodeTypes.hpp"
#include "Utility/ErrorManager.h"

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


	CNodeBase* Run()
	{
		MARK_FUNCTION_ERROR_MANAGEMENT;

		CNodeBase* result = GetExpression();

		// Checks for errors from parsing
		if (g_ErrorMgr->GetLastError() != nullptr)
		{
			return nullptr;
		}

		// Check that we actually reached end of the file/string
		// Otherwise there was an error at some point
		if (m_CurrentToken->Type() != TYPE_EOF)
		{
			g_ErrorMgr->Create<CError>("Invalid Syntax", "Expected operator ('+', '-', '*', '/')", m_CurrentToken->Start(), m_CurrentToken->End());
			return nullptr;
		}

		return result;
	}


	CToken* Advance();


	template <class NodeTy, class ...Args>
	[[nodiscard]] NodeTy* CreateNode(Args ...nodeArgs)
	{
		// Create new node
		m_Nodes.emplace_back(std::make_unique<NodeTy>(nodeArgs...));

		// Get raw ptr to node and return
		return static_cast<NodeTy*>(m_Nodes.back().get());
	}


	template <class NodeTy, class RetTy, class ...Args>
	[[nodiscard]] RetTy* CreateNode(Args ...nodeArgs)
	{
		// Create new node
		m_Nodes.emplace_back(std::make_unique<NodeTy>(nodeArgs...));

		// Get raw ptr to node and return
		return static_cast<RetTy*>(m_Nodes.back().get());
	}


	[[nodiscard]] CNodeBase* GetFactor();
	[[nodiscard]] CNodeBase* GetTerm();
	[[nodiscard]] CNodeBase* GetExpression();


	// Protected functions
protected:
	// Member variables
private:
	std::vector<CToken> m_Tokens;
	std::vector<std::unique_ptr<CNodeBase>> m_Nodes;
	CToken* m_CurrentToken;
	i32 m_TokenIndex;
};
