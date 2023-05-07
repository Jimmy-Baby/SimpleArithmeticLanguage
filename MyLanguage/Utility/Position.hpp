#pragma once

class CPosition
{
public:
	explicit CPosition(const i32 index,
	                   const i32 lineNum,
	                   const i32 columnNum,
	                   std::string fileName,
	                   std::string fileText)
		: m_Index(index),
		  m_LineNum(lineNum),
		  m_ColumnNum(columnNum),
		  m_FileName(std::move(fileName)),
		  m_Input(std::move(fileText))
	{
	}


	CPosition* Advance(const char currentChar = '\0')
	{
		m_Index++;
		m_ColumnNum++;

		if (currentChar == '\n')
		{
			m_LineNum++;
			m_ColumnNum = 0;
		}

		return this;
	}


	[[nodiscard]] i32 Index() const
	{
		return m_Index;
	}


	[[nodiscard]] i32 LineNum() const
	{
		return m_LineNum;
	}


	[[nodiscard]] i32 ColumnNum() const
	{
		return m_ColumnNum;
	}


	[[nodiscard]] std::string FileName() const
	{
		return m_FileName;
	}


	[[nodiscard]] std::string Input() const
	{
		return m_Input;
	}


private:
	i32 m_Index;
	i32 m_LineNum;
	i32 m_ColumnNum;
	std::string m_FileName;
	std::string m_Input;
};
