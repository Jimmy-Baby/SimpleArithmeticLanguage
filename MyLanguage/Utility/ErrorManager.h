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

class CFunctionErrorMarker final : public CErrorObjectBase
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

class CError final : public CErrorObjectBase
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

	[[nodiscard]] std::string StringWithArrows(const std::string& text, const CPosition& start, const CPosition& end) const;

	void Print() override
	{
		MyLang::Print("\n%s (File: %s (Line %d))\n", m_ErrorName.c_str(), m_Start.FileName().c_str(), m_Start.LineNum() + 1);
		MyLang::Print("%s %s\n\n", StringWithArrows(m_Start.Input(), m_Start, m_End).c_str(), m_Details.c_str());
	}

	[[nodiscard]] CError* GetError() override
	{
		return this;
	}

	// Returns true if an error is present
	[[nodiscard]] bool HasError() const
	{
		return !m_ErrorName.empty();
	}

	bool operator()() const
	{
		return HasError();
	}

	// Protected fields and functions
protected:
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

	// If an error related to the last registered function:
	// - Was found, then we return the last error reported
	// - Was not found, then we return nullptr
	[[nodiscard]] CError* GetLastError() const;

	// Removes all error(s) and the marker before those error(s),
	// at the top of the error object stack
	void RemoveLastFunction();

	// Returns true if no error is found from the last registered function
	//
	// printOnError = Call the base Print() function on an error object, if there are any found for the last registered function
	// clearOnError = Clear all error information if an error is found for the last registered function
	// removeLastOnSuccess = Clear information pertaining to last registered error function, if there is no error found for it
	bool CheckLastError(bool printOnError = false, bool clearOnError = false, bool removeLastOnSuccess = false);

	// Create a new error object
	template <class ObjectTy, class... Args>
	ObjectTy* Create(Args... args)
	{
		m_ErrorObjects.push_back(std::make_unique<ObjectTy>(args...));
		return dynamic_cast<ObjectTy*>(m_ErrorObjects.back().get());
	}

	// Clear all error related objects
	void Clear()
	{
		m_ErrorObjects.clear();
		m_TmpErrorPool.clear();
	}

	// Protected fields and functions
protected:
	std::vector<std::unique_ptr<CErrorObjectBase>> m_ErrorObjects;
	std::vector<std::unique_ptr<CError>> m_TmpErrorPool;
};

inline std::unique_ptr<CErrorManager> g_ErrorMgr = std::make_unique<CErrorManager>();

#define MARK_FUNCTION_ERROR_MANAGEMENT g_ErrorMgr->Create<CFunctionErrorMarker>()
