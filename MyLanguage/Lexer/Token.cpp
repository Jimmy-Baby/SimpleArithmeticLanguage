#include "Pch.h"

#include "Token.h"
#include "../IO/Console.h"

Token::Token(const ETokenType Type, std::string Value, Position StartPos, Position EndPos)
	: Type(Type),
	  Value(std::move(Value)),
	  Start(std::move(StartPos)),
	  End(std::move(EndPos))
{
	if (End.Index == -1)
	{
		End = Start;
		End.Advance();
	}
}

std::string Token::GetPrintableTokenString() const
{
	if (!Value.empty())
	{
		return std::format("[{}:{}]", GTokenTypeNames[Type], Value.c_str());
	}

	return std::format("[{}]", GTokenTypeNames[Type]);
}

void Token::Print()
{
	ArithmeticInterpreter::Print(GetPrintableTokenString());
}
