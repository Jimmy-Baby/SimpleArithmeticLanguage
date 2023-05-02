#pragma once

#include "Printable.hpp"
#include "IO/Console.hpp"
#include "Utility/Position.hpp"

class CError final : public CPrintable
{
public:
	CError() // Constructor for no error
		: CError("", "", { 0, 0, 0, "", "" }, { 0, 0, 0, "", "" })
	{
	}


	explicit CError(std::string errorName,
	                std::string details,
	                CPosition posStart,
	                CPosition posEnd)
		: m_ErrorName(std::move(errorName)),
		  m_Details(std::move(details)),
		  m_Start(std::move(posStart)),
		  m_End(std::move(posEnd))
	{
		m_ErrorPresent = !m_ErrorName.empty();
	}


	void Print() override
	{
		MyLang::Print("%s: %s\n", m_ErrorName.c_str(), m_Details.c_str());
		MyLang::Print("File: %s (Line %d)\n", m_Start.FileName().c_str(), m_Start.LineNum() + 1);
	}


	[[nodiscard]] bool ErrorPresent() const
	{
		return m_ErrorPresent;
	}


	[[nodiscard]] bool operator()() const
	{
		return ErrorPresent();
	}


private:
	std::string m_ErrorName;
	std::string m_Details;
	CPosition m_Start;
	CPosition m_End;
	bool m_ErrorPresent;
};
