#pragma once

#include "Printable.h"
#include "../IO/Console.h"
#include "Utility/Position.h"

class CErrorObjectBase : public Printable
{
public:
	explicit CErrorObjectBase(const bool IsFunctionMarker)
		: IsFnMarker(IsFunctionMarker)
	{
	}

	[[nodiscard]] bool IsError() const
	{
		return IsFnMarker == false;
	}

	[[nodiscard]] virtual class Error* GetError() = 0;

private:
	bool IsFnMarker;
};

class CFunctionErrorMarker final : public CErrorObjectBase
{
public:
	CFunctionErrorMarker()
		: CErrorObjectBase(true)
	{
	}

	[[nodiscard]] Error* GetError() override
	{
		return nullptr;
	}

	virtual void Print() override
	{
	}
};

class Error final : public CErrorObjectBase
{
public:
	Error() // Constructor for no error
		: Error("", "", Position(0, 0, 0, ""), Position(0, 0, 0, ""))
	{
	}

	explicit Error(std::string ErrorName,
	               std::string Details,
	               Position PosStart,
	               Position PosEnd)
		: CErrorObjectBase(false),
		  ErrorName(std::move(ErrorName)),
		  Details(std::move(Details)),
		  Start(std::move(PosStart)),
		  End(std::move(PosEnd))
	{
	}

	[[nodiscard]] std::string StringWithArrows(const std::string& Text, const Position& Start, const Position& End) const;

	virtual void Print() override
	{
		ArithmeticInterpreter::Print("\n%s (Input (Line %d))\n", ErrorName.c_str(), Start.LineNumber + 1);
		ArithmeticInterpreter::Print("%s %s\n\n", StringWithArrows(Start.Input, Start, End).c_str(), Details.c_str());
	}

	[[nodiscard]] Error* GetError() override
	{
		return this;
	}

	// Returns true if an error is present
	[[nodiscard]] bool HasError() const
	{
		return !ErrorName.empty();
	}

	bool operator()() const
	{
		return HasError();
	}

	// Protected fields and functions
protected:
	std::string ErrorName;
	std::string Details;
	Position Start;
	Position End;
};

class ErrorManager
{
	// Access functions
public:
	ErrorManager() = default;
	ErrorManager(const ErrorManager& Source) = delete;
	ErrorManager& operator=(const ErrorManager& Source) = delete;
	ErrorManager(ErrorManager&& Source) = delete;
	ErrorManager& operator=(ErrorManager&& Source) = delete;
	~ErrorManager() = default;

	// If an error related to the last registered function:
	// - Was found, then we return the last error reported
	// - Was not found, then we return nullptr
	[[nodiscard]] Error* GetLastError() const;

	// Removes all error(s) and the marker before those error(s),
	// at the top of the error object stack
	void RemoveLastFunction();

	// Returns true if no error is found from the last registered function
	//
	// printOnError = Call the base Print() function on an error object, if there are any found for the last registered function
	// clearOnError = Clear all error information if an error is found for the last registered function
	// removeLastOnSuccess = Clear information pertaining to last registered error function, if there is no error found for it
	bool CheckLastError(bool PrintOnError = false, bool ClearOnError = false, bool RemoveLastOnSuccess = false);

	// Create a new error object
	template <class ObjectTy, class... ArgTys>
	ObjectTy* Create(ArgTys... Args)
	{
		ObjectBases.push_back(std::make_unique<ObjectTy>(Args...));
		return dynamic_cast<ObjectTy*>(ObjectBases.back().get());
	}

	// Clear all error related objects
	void Clear()
	{
		ObjectBases.clear();
		TmpErrorPool.clear();
	}

	// Protected fields and functions
protected:
	std::vector<std::unique_ptr<CErrorObjectBase>> ObjectBases;
	std::vector<std::unique_ptr<Error>> TmpErrorPool;
};

inline std::unique_ptr<ErrorManager> GErrorMgr = std::make_unique<ErrorManager>();

#define MARK_FUNCTION_ERROR_MANAGEMENT GErrorMgr->Create<CFunctionErrorMarker>()
