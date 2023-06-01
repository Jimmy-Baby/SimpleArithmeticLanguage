#pragma once

#include "Parser/NodeTypes.hpp"
#include "Number.hpp"

namespace Interpreter
{
	CNumber VisitRoot(CNodeBase* root);
	CNumber VisitNumberNode(const CNumberNode* node);
	CNumber VisitBinaryOperator(const CBinaryOpNode* node);
	CNumber VisitUnaryOperator(const CUnaryOpNode* node);
}
