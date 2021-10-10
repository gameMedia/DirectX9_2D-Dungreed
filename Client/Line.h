#pragma once

class CLine
{
public:
	CLine();
	CLine(LINEINFO& rLine);
	~CLine();
public:
	const LINEINFO* Get_Line() { return &m_tLine; }

public:
	void Set_RPoint(LINEPOS& tPoint)
	{
		m_tLine.RPoint.Linepos.x = tPoint.Linepos.x;
		m_tLine.RPoint.Linepos.y = tPoint.Linepos.y;
	}

public:
	void Render();
public:
	LINEINFO m_tLine;
};

