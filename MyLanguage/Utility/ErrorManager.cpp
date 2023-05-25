// Precompiled headers
#include "Pch.hpp"

#include "ErrorManager.h"

[[nodiscard]] std::string CError::StringWithArrows(const std::string& text, const CPosition& start, const CPosition& end) const
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

[[nodiscard]] CError* CErrorManager::GetLastError() const
{
	if (m_ErrorObjects.empty())
	{
		return nullptr;
	}

	const auto& lastError = m_ErrorObjects.back();

	if (lastError->IsError())
	{
		return dynamic_cast<CError*>(lastError.get());
	}

	return nullptr;
}

void CErrorManager::RemoveLastFunction()
{
	auto lastError = std::prev(m_ErrorObjects.end());

	if (lastError->get()->IsError())
	{
		bool deletedMarker = false;
		while (!deletedMarker)
		{
			lastError = --m_ErrorObjects.erase(lastError);

			if (!lastError->get()->IsError())
			{
				deletedMarker = true;
			}
		}

		return;
	}

	// If no errors, then just delete our marker
	m_ErrorObjects.erase(lastError);
}

bool CErrorManager::CheckLastError(const bool printOnError, const bool clearOnError, const bool removeLastOnSuccess)
{
	if (CError* err = GetLastError();
		err != nullptr)
	{
		if (printOnError)
		{
			err->Print();
		}

		if (clearOnError)
		{
			Clear();
		}

		return false;
	}

	if (removeLastOnSuccess && !m_ErrorObjects.empty())
	{
		RemoveLastFunction();
	}

	return true;
}
