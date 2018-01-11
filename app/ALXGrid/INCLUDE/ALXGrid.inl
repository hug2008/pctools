// Inlines for CALXGrid.h
//
/////////////////////////////////////////////////////////////////////////////

// Получаем активную колонку, столбец
	inline int CALXGrid::GetActiveCol()			{ return GetGridCol(CALXGridCore::GetActiveCol()); };
	inline int CALXGrid::GetActiveRow()			{ return GetGridRow(CALXGridCore::GetActiveRow()); };

// Активизируем колонку, столбец
	inline BOOL CALXGrid::SetActiveCell(int nCol, int nRow)		{ return CALXGridCore::SetActiveCell(nCol + GetFrozenCol(), nRow + GetHeaderRow()); };
	inline BOOL CALXGrid::SetActiveNextCol( void )				{ return CALXGrid::SetActiveCell(CALXGrid::GetActiveCol() + 1, CALXGrid::GetActiveRow()); };
	inline BOOL CALXGrid::SetActivePrevCol( void )				{ return CALXGrid::SetActiveCell(CALXGrid::GetActiveCol() - 1, CALXGrid::GetActiveRow()); };
	inline BOOL CALXGrid::SetActiveNextRow( void )				{ return CALXGrid::SetActiveCell(CALXGrid::GetActiveCol(), CALXGrid::GetActiveRow() + 1); };
	inline BOOL CALXGrid::SetActivePrevRow( void )				{ return CALXGrid::SetActiveCell(CALXGrid::GetActiveCol(), CALXGrid::GetActiveRow() - 1); };
	inline BOOL CALXGrid::SetActiveCol(int nNewActiveCol)		{ return CALXGrid::SetActiveCell(nNewActiveCol, CALXGrid::GetActiveRow()); };
	inline BOOL CALXGrid::SetActiveRow(int nNewActiveRow)		{ return CALXGrid::SetActiveCell(CALXGrid::GetActiveCol(), nNewActiveRow); };

/////////////////////////////////////////////////////////////////////////////

