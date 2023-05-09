#pragma once
#pragma warning (disable : 26812)

#include "IO/Console.hpp"
#include "Utility/Position.hpp"
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
	TYPE_RBRACKET,
	TYPE_EOF
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
	"RBRACKET",
	"TYPE_EOF"
};

class CToken final : public CPrintable
{
public:
	explicit CToken(const ETokenType type,
	                std::string value = std::string(""),
	                CPosition startPos = CPosition(-1, 0, 0, "", ""),
	                CPosition endPos = CPosition(-1, 0, 0, "", ""))
		: m_Type(type),
		  m_Value(std::move(value)),
		  m_Start(std::move(startPos)),
		  m_End(std::move(endPos))
	{
		if (m_End.Index() == -1)
		{
			m_End = m_Start;
			m_End.Advance();
		}
	}


	[[nodiscard]] std::string GetPrintableTokenString() const
	{
		if (!m_Value.empty())
		{
			return std::format("[{}:{}]", g_TokenTypeNames[m_Type], m_Value.c_str());
		}

		return std::format("[{}]", g_TokenTypeNames[m_Type]);
	}


	[[nodiscard]] ETokenType Type() const
	{
		return m_Type;
	}


	void Print() override
	{
		MyLang::Print(GetPrintableTokenString());
	}


	[[nodiscard]] const CPosition& Start()
	{
		return m_Start;
	}


	[[nodiscard]] const CPosition& End()
	{
		return m_End;
	}


	std::string& GetValue()
	{
		return m_Value;
	}


private:
	ETokenType m_Type;
	std::string m_Value;
	CPosition m_Start;
	CPosition m_End;
};
