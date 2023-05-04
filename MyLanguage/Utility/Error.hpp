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


	[[nodiscard]] std::string StringWithArrows(const std::string& text, const CPosition& start, const CPosition& end) const
	{
		std::string result;

		u64 indexStart = std::max(static_cast<i64>(text.rfind('\n', start.Index())), 0i64);
		u64 indexEnd = text.find('\n', indexStart + 1);

		if (indexEnd == std::string::npos)
		{
			indexEnd = text.length();
		}

		const i32 lineCount = end.LineNum() - start.LineNum() + 1;

		for (int i = 0; i < lineCount; ++i)
		{
			std::string line = text.substr(indexStart, indexEnd - indexStart);

			i32 columnStart;
			i32 columnEnd;

			if (i == 0)
			{
				columnStart = start.ColumnNum();
			}
			else
			{
				columnStart = 0;
			}

			if (i == lineCount - 1)
			{
				columnEnd = end.ColumnNum();
			}
			else
			{
				columnEnd = static_cast<i32>(line.length()) - 1;
			}

			result += line + '\n';

			// Add out whitespace
			for (int j = 0; j < columnStart; ++j)
			{
				result += ' ';
			}

			// Add our arrows
			for (int j = 0; j < columnEnd - columnStart; ++j)
			{
				result += '^';
			}

			// Recalculate indexes
			indexStart = indexEnd;
			indexEnd = text.find('\n', indexStart + 1);

			if (indexEnd == std::string::npos)
			{
				indexEnd = text.length();
			}
		}

		std::erase(result, '\t');
		return result;
	}


	void Print() override
	{
		MyLang::Print("%s: %s\n", m_ErrorName.c_str(), m_Details.c_str());
		MyLang::Print("File: %s (Line %d)\n", m_Start.FileName().c_str(), m_Start.LineNum() + 1);

		const std::string stringArrowsText = StringWithArrows(m_Start.Input(), m_Start, m_End);

		MyLang::Print("\n%s\n", stringArrowsText.c_str());
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
