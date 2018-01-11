// Inlines for CALXGridCore.h
//
/////////////////////////////////////////////////////////////////////////////

// Количество строк в шапке и неподвижных столбцов 
	// Получаем количество неподвижных столбцов
	inline int CALXGridCore::GetFrozenCol()	{ return m_CountFrozenCols; }
	// Получаем количество строк в шапке
	inline int CALXGridCore::GetHeaderRow()	{ return m_CountHeaderRows; }

	inline int CALXGridCore::GetGridRow(int nRow) { return nRow - m_CountHeaderRows; };
	inline int CALXGridCore::GetGridCol(int nCol) { return nCol - m_CountFrozenCols; };

// Получаем активную колонку, столбец
	inline int CALXGridCore::GetActiveCol()		{ return m_nActiveCol; };
	inline int CALXGridCore::GetActiveRow()		{ return m_nActiveRow; };

	// Получаем общее количество строк
	inline int CALXGridCore::GetAllRowCount()	{ return GetGridRowCount() + m_CountHeaderRows; };
	// Получаем общее количество столбцов
	inline int CALXGridCore::GetAllColCount(int nHeadRow)	{ return GetHeadColCount(nHeadRow) + m_CountFrozenCols; };

// Область выделения

	// Пустая ли область
	inline BOOL CALXGridCore::ALXSelRangeCell::IsEmpty() {	return (m_RightCol < 0 || m_BottomRow < 0);	}
	// Принадлежит ли ячейка области
	inline BOOL CALXGridCore::ALXSelRangeCell::InRange(int nCol, int nRow)
	{
		return (nCol >= m_LeftCol && nCol <= m_RightCol && nRow >= m_TopRow && nRow <= m_BottomRow);
	}


/////////////////////////////////////////////////////////////////////////////
