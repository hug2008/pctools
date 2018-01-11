// Inlines for CALXGridCore.h
//
/////////////////////////////////////////////////////////////////////////////

// ���������� ����� � ����� � ����������� �������� 
	// �������� ���������� ����������� ��������
	inline int CALXGridCore::GetFrozenCol()	{ return m_CountFrozenCols; }
	// �������� ���������� ����� � �����
	inline int CALXGridCore::GetHeaderRow()	{ return m_CountHeaderRows; }

	inline int CALXGridCore::GetGridRow(int nRow) { return nRow - m_CountHeaderRows; };
	inline int CALXGridCore::GetGridCol(int nCol) { return nCol - m_CountFrozenCols; };

// �������� �������� �������, �������
	inline int CALXGridCore::GetActiveCol()		{ return m_nActiveCol; };
	inline int CALXGridCore::GetActiveRow()		{ return m_nActiveRow; };

	// �������� ����� ���������� �����
	inline int CALXGridCore::GetAllRowCount()	{ return GetGridRowCount() + m_CountHeaderRows; };
	// �������� ����� ���������� ��������
	inline int CALXGridCore::GetAllColCount(int nHeadRow)	{ return GetHeadColCount(nHeadRow) + m_CountFrozenCols; };

// ������� ���������

	// ������ �� �������
	inline BOOL CALXGridCore::ALXSelRangeCell::IsEmpty() {	return (m_RightCol < 0 || m_BottomRow < 0);	}
	// ����������� �� ������ �������
	inline BOOL CALXGridCore::ALXSelRangeCell::InRange(int nCol, int nRow)
	{
		return (nCol >= m_LeftCol && nCol <= m_RightCol && nRow >= m_TopRow && nRow <= m_BottomRow);
	}


/////////////////////////////////////////////////////////////////////////////
