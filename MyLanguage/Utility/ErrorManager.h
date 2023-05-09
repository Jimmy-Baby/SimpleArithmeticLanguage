#pragma once

#include "Printable.hpp"
#include "IO/Console.hpp"
#include "Utility/Position.hpp"

class CErrorObjectBase : public CPrintable
{
public:
	explicit CErrorObjectBase(const bool isFunctionMarker)
		: m_IsFnMarker(isFunctionMarker)
	{
	}


	[[nodiscard]] bool IsError() const
	{
		return m_IsFnMarker == false;
	}


	[[nodiscard]] virtual class CError* GetError() = 0;

private:
	bool m_IsFnMarker;
};

class CFunctionErrorMarker : public CErrorObjectBase
{
public:
	CFunctionErrorMarker()
		: CErrorObjectBase(true)
	{
	}


	[[nodiscard]] CError* GetError() override
	{
		return nullptr;
	}


	void Print() override
	{
	}
};

class CError : public CErrorObjectBase
{
public:
	CError() // Constructor for no error
		: CError("", "", CPosition(0, 0, 0, "", ""), CPosition(0, 0, 0, "", ""))
	{
	}


	explicit CError(std::string errorName,
	                std::string details,
	                CPosition posStart,
	                CPosition posEnd)
		: CErrorObjectBase(false),
		  m_ErrorName(std::move(errorName)),
		  m_Details(std::move(details)),
		  m_Start(std::move(posStart)),
		  m_End(std::move(posEnd))
	{
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
		MyLang::Print("\n%s (File: %s (Line %d))\n", m_ErrorName.c_str(), m_Start.FileName().c_str(), m_Start.LineNum() + 1);
		MyLang::Print("%s %s\n\n", StringWithArrows(m_Start.Input(), m_Start, m_End).c_str(), m_Details.c_str());
	}


	[[nodiscard]] CError* GetError() override
	{
		return this;
	}


	[[nodiscard]] bool HasError() const
	{
		return !m_ErrorName.empty();
	}


	bool operator()() const
	{
		return HasError();
	}


private:
	std::string m_ErrorName;
	std::string m_Details;
	CPosition m_Start;
	CPosition m_End;
};

class CErrorManager
{
	// Access functions
public:
	CErrorManager() = default;
	CErrorManager(const CErrorManager& source) = delete;
	CErrorManager& operator=(const CErrorManager& source) = delete;
	CErrorManager(CErrorManager&& source) = delete;
	CErrorManager& operator=(CErrorManager&& source) = delete;
	~CErrorManager() = default;


	template <class ObjectTy, class ...Args>
	ObjectTy* Create(Args ...args)
	{
		m_ErrorObjects.push_back(std::make_unique<ObjectTy>(args...));
		return dynamic_cast<ObjectTy*>(m_ErrorObjects.back().get());
	}


	// If an error related to the last registered function:
	// - Was found, then we return the last error reported
	// - Was not found, then we return nullptr
	[[nodiscard]] CError* GetLastError() const
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


	// Removes all error(s) and the marker before those error(s),
	// at the top of the error object stack
	void RemoveLastFunction()
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


	void Clear()
	{
		m_ErrorObjects.clear();
		m_TmpErrorPool.clear();
	}


	// Returns true if no error is found from the last registered function
	//
	// printOnError = Call the base Print() function on an error object, if there are any found for the last registered function
	// clearOnError = Clear all error information if an error is found for the last registered function
	// removeLastOnSuccess = Clear information pertaining to last registered error function, if there is no error found for it
	bool CheckLastError(const bool printOnError = false, const bool clearOnError = false, const bool removeLastOnSuccess = false)
	{
		CError* err = GetLastError();

		if (err != nullptr)
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


	// Protected functions
protected:
	// Member variables
private:
	std::vector<std::unique_ptr<CErrorObjectBase>> m_ErrorObjects;
	std::vector<std::unique_ptr<CError>> m_TmpErrorPool;
};

inline std::unique_ptr<CErrorManager> g_ErrorMgr = std::make_unique<CErrorManager>();

#define MARK_FUNCTION_ERROR_MANAGEMENT g_ErrorMgr->Create<CFunctionErrorMarker>()
