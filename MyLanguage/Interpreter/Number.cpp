#include "Pch.h"

#include "Number.h"

Number::Number(const int64_t Value)
	: Value(Value)
{
}

[[nodiscard]] Number Number::AddedTo(const Number& Other) const
{
	return Number(Value + Other.Value);
}

[[nodiscard]] Number Number::SubtractedBy(const Number& Other) const
{
	return Number(Value - Other.Value);
}

[[nodiscard]] Number Number::MultipliedBy(const Number& Other) const
{
	return Number(Value * Other.Value);
}

[[nodiscard]] Number Number::DividedBy(const Number& Other) const
{
	return Number(Value / Other.Value);
}
