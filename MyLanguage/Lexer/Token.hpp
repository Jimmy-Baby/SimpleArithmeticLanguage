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


	[[nodiscard]] std::string GetPrintableTokenString() const
	{
		if (!m_Value.empty())
		{
			return std::format("[{}:{}]", g_TokenTypeNames[m_Type], m_Value.c_str());
		}

		return std::format("[{}]", g_TokenTypeNames[m_Type]);
	}


	ETokenType Type() const
	{
		return m_Type;
	}


	void Print() override
	{
		MyLang::Print(GetPrintableTokenString());
	}


private:
	ETokenType m_Type;
	std::string m_Value;
};
