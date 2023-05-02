#pragma once

class CPrintable
{
public:
	virtual ~CPrintable() = default;
	virtual void Print() = 0;
};
