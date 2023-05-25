#pragma once

#include "Utility/Position.hpp"

class CNumber
{
	// Access functions
public:
	explicit CNumber(const i64 value)
		: m_Value(value),
		  m_Start(-1, 0, 0, "", ""),
		  m_End(-1, 0, 0, "", "")
	{
	}

	CNumber(const CNumber& source) = default;
	CNumber& operator=(const CNumber& source) = default;
	CNumber(CNumber&& source) = default;
	CNumber& operator=(CNumber&& source) = default;
	~CNumber() = default;

	void SetPosition(CPosition start = CPosition(-1, 0, 0, "", ""), CPosition end = CPosition(-1, 0, 0, "", ""))
	{
		m_Start = std::move(start);
		m_End = std::move(end);
	}

	[[nodiscard]] CNumber AddedTo(const CNumber& other) const
	{
		return CNumber(m_Value + other.GetValue());
	}

	[[nodiscard]] CNumber SubtractedBy(const CNumber& other) const
	{
		return CNumber(m_Value - other.GetValue());
	}

	[[nodiscard]] CNumber MultipliedBy(const CNumber& other) const
	{
		return CNumber(m_Value * other.GetValue());
	}

	[[nodiscard]] CNumber DividedBy(const CNumber& other) const
	{
		return CNumber(m_Value / other.GetValue());
	}

	[[nodiscard]] i64 GetValue() const
	{
		return m_Value;
	}

	// Protected fields and functions
protected:
	i64 m_Value;
	CPosition m_Start;
	CPosition m_End;
};
