#include "Pch.hpp"

#include "Interpreter.hpp"
#include "Utility/ErrorManager.h"

namespace Interpreter
{
	CNumber VisitRoot(CNodeBase* root)
	{
		switch (root->GetType())
		{
		case NODE_TYPE_BINARY_OP:
			return VisitBinaryOperator(dynamic_cast<CBinaryOpNode*>(root));

		case NODE_TYPE_NUMBER:
			return VisitNumberNode(dynamic_cast<CNumberNode*>(root));

		case NODE_TYPE_UNARY_OP:
			return VisitUnaryOperator(dynamic_cast<CUnaryOpNode*>(root));
		}

		return CNumber(0);
	}

	CNumber VisitNumberNode(const CNumberNode* node)
	{
		CToken* nodeToken = node->GetToken();
		CNumber numberOut(node->GetValueNumber());

		numberOut.SetPosition(nodeToken->Start(), nodeToken->End());
		return numberOut;
	}

	CNumber VisitBinaryOperator(const CBinaryOpNode* node)
	{
		MARK_FUNCTION_ERROR_MANAGEMENT;

		const CNumber left = VisitRoot(node->GetLeftNode());

		if (!g_ErrorMgr->CheckLastError(false, false, true))
		{
			return CNumber(0);
		}

		const CNumber right = VisitRoot(node->GetRightNode());

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

	CNumber VisitUnaryOperator(const CUnaryOpNode* node)
	{
		MARK_FUNCTION_ERROR_MANAGEMENT;

		const CNumber child = VisitRoot(node->GetChildNode());

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
}
