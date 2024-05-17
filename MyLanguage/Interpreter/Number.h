#pragma once

class Number
{
	// Access functions
public:
	explicit Number(int64_t Value);

	[[nodiscard]] Number AddedTo(const Number& Other) const;
	[[nodiscard]] Number SubtractedBy(const Number& Other) const;
	[[nodiscard]] Number MultipliedBy(const Number& Other) const;
	[[nodiscard]] Number DividedBy(const Number& Other) const;

	int64_t Value;
};
