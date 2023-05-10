#pragma once

#include "Parser/NodeTypes.hpp"
#include "Number.hpp"
#include "Utility/ErrorManager.h"

class CInterpreter
{
	// Access functions
public:
	CInterpreter() = default;
	CInterpreter(const CInterpreter& source) = delete;
	CInterpreter& operator=(const CInterpreter& source) = delete;
	CInterpreter(CInterpreter&& source) = delete;
	CInterpreter& operator=(CInterpreter&& source) = delete;
	~CInterpreter() = default;


	static CNumber Visit(CNodeBase* node)
	{
		switch (node->GetType())
		{
		case NODE_TYPE_BINARY_OP:
			return VisitBinaryOperator(dynamic_cast<CBinaryOpNode*>(node));

		case NODE_TYPE_NUMBER:
			return VisitNumberNode(dynamic_cast<CNumberNode*>(node));

		case NODE_TYPE_UNARY_OP:
			return VisitUnaryOperator(dynamic_cast<CUnaryOpNode*>(node));
		}

		return CNumber(0);
	}


	static CNumber VisitNumberNode(const CNumberNode* node)
	{
		CToken* nodeToken = node->GetToken();
		CNumber numberOut(node->GetValueNumber());

		numberOut.SetPosition(nodeToken->Start(), nodeToken->End());
		return numberOut;
	}


	static CNumber VisitBinaryOperator(const CBinaryOpNode* node)
	{
		MARK_FUNCTION_ERROR_MANAGEMENT;

		const CNumber left = Visit(node->GetLeftNode());

		if (!g_ErrorMgr->CheckLastError(false, false, true))
		{
			return CNumber(0);
		}

		const CNumber right = Visit(node->GetRightNode());

		if (!g_ErrorMgr->CheckLastError(false, false, true))
		{
			return CNumber(0);
		}

		switch (node->GetToken()->Type())
		{
		case TYPE_PLUS:
			return left.AddedTo(right);

		case TYPE_MINUS:
			return left.SubtractedBy(right);

		case TYPE_MUL:
			return left.MultipliedBy(right);

		case TYPE_DIV:
			if (right.GetValue() == 0)
			{
				g_ErrorMgr->Create<CError>("Runtime Error", "Integer Division by 0", node->GetStart(), node->GetEnd());
				return CNumber(0);
			}

			return left.DividedBy(right);

		case TYPE_FLOAT:
		case TYPE_LBRACKET:
		case TYPE_RBRACKET:
		case TYPE_EOF:
		case TYPE_INT:
			break;
		}

		return CNumber(0);
	}


	static CNumber VisitUnaryOperator(const CUnaryOpNode* node)
	{
		MARK_FUNCTION_ERROR_MANAGEMENT;

		const CNumber child = Visit(node->GetChildNode());

		if (!g_ErrorMgr->CheckLastError(false, false, true))
		{
			return CNumber(0);
		}

		if (node->GetToken()->Type() == TYPE_MINUS)
		{
			return child.MultipliedBy(CNumber(-1));
		}

		return CNumber(0);
	}
};
