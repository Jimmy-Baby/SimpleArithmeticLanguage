#include "Pch.h"

#include "Interpreter.h"
#include "Utility/ErrorManager.h"

namespace Interpreter
{
	Number Visit(NodeBase* Root)
	{
		switch (Root->Type)
		{
		case NODE_TYPE_BINARY_OP:
			return VisitBinaryOperator(dynamic_cast<BinaryOpNode*>(Root));

		case NODE_TYPE_NUMBER:
			return VisitNumberNode(dynamic_cast<NumberNode*>(Root));

		case NODE_TYPE_UNARY_OP:
			return VisitUnaryOperator(dynamic_cast<UnaryOpNode*>(Root));
		}

		return Number(0);
	}

	Number VisitNumberNode(const NumberNode* Node)
	{
		return Number(Node->GetValueNumber());
	}

	Number VisitBinaryOperator(const BinaryOpNode* Node)
	{
		MARK_FUNCTION_ERROR_MANAGEMENT;

		const Number Left = Visit(Node->LeftNode);

		if (!GErrorMgr->CheckLastError(false, false, true))
		{
			return Number(0);
		}

		const Number Right = Visit(Node->RightNode);

		if (!GErrorMgr->CheckLastError(false, false, true))
		{
			return Number(0);
		}

		switch (Node->Token->Type)
		{
		case TYPE_PLUS:
			return Left.AddedTo(Right);

		case TYPE_MINUS:
			return Left.SubtractedBy(Right);

		case TYPE_MUL:
			return Left.MultipliedBy(Right);

		case TYPE_DIV:
			if (Right.Value == 0)
			{
				GErrorMgr->Create<Error>("Runtime Error", "Integer Division by 0", Node->Start, Node->End);
				return Number(0);
			}

			return Left.DividedBy(Right);

		case TYPE_FLOAT:
		case TYPE_LBRACKET:
		case TYPE_RBRACKET:
		case TYPE_EOF:
		case TYPE_INT:
			break;
		}

		return Number(0);
	}

	Number VisitUnaryOperator(const UnaryOpNode* Node)
	{
		MARK_FUNCTION_ERROR_MANAGEMENT;

		const Number Child = Visit(Node->ChildNode);

		if (!GErrorMgr->CheckLastError(false, false, true))
		{
			return Number(0);
		}

		if (Node->OperatorToken->Type == TYPE_MINUS)
		{
			return Child.MultipliedBy(Number(-1));
		}

		return Number(0);
	}
}
