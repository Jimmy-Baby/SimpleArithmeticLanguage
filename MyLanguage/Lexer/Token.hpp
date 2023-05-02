#pragma once

#include "IO/Console.hpp"
#include "Utility/Printable.hpp"

enum ETokenType
{
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_PLUS,
	TYPE_MINUS,
	TYPE_MUL,
	TYPE_DIV,
	TYPE_LBRACKET,
	TYPE_RBRACKET
};

inline const char* g_TokenTypeNames[] =
{
	"INT",
	"FLOAT",
	"PLUS",
	"MINUS",
	"MUL",
	"DIV",
	"LBRACKET",
	"RBRACKET"
};

class CToken final : public CPrintable
{
public:
	explicit CToken(const ETokenType type, std::string value = "")
		: m_Type(type),
		  m_Value(std::move(value))
	{
	}


	void Print() override
	{
		if (!m_Value.empty())
		{
			MyLang::Print("[%s:%s]", g_TokenTypeNames[m_Type], m_Value.c_str());
		}
		else
		{
			MyLang::Print("[%s]", g_TokenTypeNames[m_Type]);
		}
	}


private:
	ETokenType m_Type;
	std::string m_Value;
};
