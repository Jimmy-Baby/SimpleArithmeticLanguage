// Precompiled headers
#include "Pch.h"

#include "ErrorManager.h"

[[nodiscard]] std::string Error::StringWithArrows(const std::string& Text, const Position& Start, const Position& End) const
{
	std::string ReturnValue;

	uint64_t IndexStart = std::max(Text.rfind('\n', Start.Index), 0ui64);
	uint64_t IndexEnd = Text.find('\n', IndexStart + 1);

	if (IndexEnd == std::string::npos)
	{
		IndexEnd = Text.length();
	}

	const int LineCount = End.LineNumber - Start.LineNumber + 1;

	for (int LineIndex = 0; LineIndex < LineCount; ++LineIndex)
	{
		std::string Line = Text.substr(IndexStart, IndexEnd - IndexStart);

		int ColumnStart;
		int ColumnEnd;

		if (LineIndex == 0)
		{
			ColumnStart = Start.ColumnNumber;
		}
		else
		{
			ColumnStart = 0;
		}

		if (LineIndex == LineCount - 1)
		{
			ColumnEnd = End.ColumnNumber;
		}
		else
		{
			ColumnEnd = static_cast<int>(Line.length()) - 1;
		}

		ReturnValue += Line + '\n';

		// Add out whitespace
		for (int ColumnIndex = 0; ColumnIndex < ColumnStart; ++ColumnIndex)
		{
			ReturnValue += ' ';
		}

		// Add our arrows
		for (int ColumnIndex = 0; ColumnIndex < ColumnEnd - ColumnStart; ++ColumnIndex)
		{
			ReturnValue += '^';
		}

		// Recalculate indexes
		IndexStart = IndexEnd;
		IndexEnd = Text.find('\n', IndexStart + 1);

		if (IndexEnd == std::string::npos)
		{
			IndexEnd = Text.length();
		}
	}

	std::erase(ReturnValue, '\t');

	return ReturnValue;
}

[[nodiscard]] Error* ErrorManager::GetLastError() const
{
	if (ObjectBases.empty())
	{
		return nullptr;
	}

	const auto& LastError = ObjectBases.back();

	if (LastError->IsError())
	{
		return dynamic_cast<Error*>(LastError.get());
	}

	return nullptr;
}

void ErrorManager::RemoveLastFunction()
{
	auto LastError = std::prev(ObjectBases.end());

	if (LastError->get()->IsError())
	{
		bool DeletedFlag = false;

		while (!DeletedFlag)
		{
			LastError = --ObjectBases.erase(LastError);

			if (!LastError->get()->IsError())
			{
				DeletedFlag = true;
			}
		}

		return;
	}

	// If no errors, then just delete our marker
	ObjectBases.erase(LastError);
}

bool ErrorManager::CheckLastError(const bool PrintOnError, const bool ClearOnError, const bool RemoveLastOnSuccess)
{
	if (Error* LastError = GetLastError();
		LastError != nullptr)
	{
		if (PrintOnError)
		{
			LastError->Print();
		}

		if (ClearOnError)
		{
			Clear();
		}

		return false;
	}

	if (RemoveLastOnSuccess && !ObjectBases.empty())
	{
		RemoveLastFunction();
	}

	return true;
}
