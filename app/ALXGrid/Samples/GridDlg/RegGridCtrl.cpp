// GridCtrl.cpp : implementation of the CRegGridCtrl class
//

#include "stdafx.h"
#include "Grid.h"
#include "RegGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegGridCtrl

IMPLEMENT_DYNCREATE(CRegGridCtrl, CALXGridCtrl)
IMPLEMENT_REGISTER(CRegGridCtrl,CS_DBLCLKS)

BEGIN_MESSAGE_MAP(CRegGridCtrl, CALXGridCtrl)
	//{{AFX_MSG_MAP(CRegGridCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegGridCtrl construction/destruction

CRegGridCtrl::CRegGridCtrl()
{
//TEXT
	AddCol(100,"Text", DT_RIGHT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);//,GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	AddCol(100,"Text", DT_CENTER | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	DefineImage(1,7,7);
	AddCol(100,"Text", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);//,GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	AddCol(100,"Text", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
// BUTTON
	// CHECKBOX
	DefineColCtrl(AddCol(100,"CheckBox3State", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_BUTTON,DFCS_BUTTON3STATE | DFCS_CHECKED | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTO3STATE | BS_FLAT);
	DefineColCtrl(AddCol(90,"CheckBox", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_BUTTON,DFCS_BUTTONCHECK | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT);
	// RADIOBUTTON
	//DefineColCtrl(AddCol(90,"RadioButton", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_BUTTON,DFCS_BUTTONRADIO | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTORADIOBUTTON | BS_FLAT);
	//DefineColCtrl(AddCol(100,"RadioImage", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_BUTTON,DFCS_BUTTONRADIOIMAGE | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTORADIOBUTTON | BS_FLAT);
	//DefineColCtrl(AddCol(100,"RadioMask", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_BUTTON,DFCS_BUTTONRADIOMASK | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTORADIOBUTTON | BS_FLAT);
	// PUSHBUTTON
	//DefineColCtrl(AddCol(50,"PushButton", DT_CENTER | DT_VCENTER, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS,DFC_BUTTON,DFCS_BUTTONPUSH),GA_BUTTONCTRL,WS_CHILD | BS_PUSHBUTTON);

// CAPTION
	//DefineColCtrl(AddCol(60,"CaptionClose", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_CAPTION,DFCS_CAPTIONCLOSE),GA_CELLCTRL);
	DefineColCtrl(AddCol(60,"CaptionHelp", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_CAPTION,DFCS_CAPTIONHELP),GA_CELLCTRL);
	//DefineColCtrl(AddCol(60,"CaptionMax", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_CAPTION,DFCS_CAPTIONMAX),GA_CELLCTRL);
	//DefineColCtrl(AddCol(60,"CaptionMin", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_CAPTION,DFCS_CAPTIONMIN),GA_CELLCTRL);
	//DefineColCtrl(AddCol(60,"CaptionRestore", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_CAPTION,DFCS_CAPTIONRESTORE),GA_CELLCTRL);

// DFC_MENU   
	//DefineColCtrl(AddCol(50,"MenuArrow", DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_MENU,DFCS_MENUARROW),GA_CELLCTRL);
	//DefineColCtrl(AddCol(50,"MenuBullet", DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_MENU,DFCS_MENUBULLET),GA_BUTTONCTRL,WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT);
	DefineColCtrl(AddCol(50,"MenuCheck", DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_MENU,DFCS_MENUCHECK),GA_CELLCTRL);

// DFC_SCROLL
	//DefineColCtrl(AddCol(60,"ScrollSizeGrip", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_SCROLL,DFCS_SCROLLSIZEGRIP),GA_EDITCTRL);
	DefineColCtrl(AddCol(60,"ScrollComboBox", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_SCROLL,DFCS_SCROLLCOMBOBOX),GA_COMBOBOXCTRL,WS_CHILD | CBS_DROPDOWN | WS_VSCROLL);
	//DefineColCtrl(AddCol(60,"ScrollRight", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_SCROLL,DFCS_SCROLLRIGHT),GA_CELLCTRL);
	//DefineColCtrl(AddCol(60,"ScrollLeft", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_SCROLL,DFCS_SCROLLLEFT),GA_CELLCTRL);
	//DefineColCtrl(AddCol(60,"ScrollDown", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_SCROLL,DFCS_SCROLLDOWN),GA_CELLCTRL);
	//DefineColCtrl(AddCol(60,"ScrollUp", DT_LEFT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS,DFC_SCROLL,DFCS_SCROLLUP),GA_CELLCTRL);

//	DefineColCtrl(AddCol(150,"������", DT_RIGHT | DT_VCENTER),GA_COMBOBOXCTRL,WS_CHILD | CBS_DROPDOWN | WS_VSCROLL);   
//	DefineColCtrl(AddCol(150,"������", DT_RIGHT | DT_VCENTER),GA_COMBOBOXCTRL,WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL);   
//	DefineColCtrl(AddCol(100,"�����", DT_LEFT | DT_VCENTER),GA_BUTTONCTRL);
//	AddCol(200, "������", DT_CENTER | DT_VCENTER);
//	AddCol(200, "�������", DT_RIGHT | DT_VCENTER);
//	InsertCol(0,80,"����");
//TEXT
	AddCol(100,"Text", DT_RIGHT | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);//,GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	AddCol(100,"Text", DT_CENTER | DT_VCENTER, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	m_BMP.LoadBitmap(IDB_BITMAP);
	SetGridRowCount(100);
}

CRegGridCtrl::~CRegGridCtrl()
{
}


BOOL CRegGridCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	return CALXGridCtrl::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRegGridCtrl diagnostics

#ifdef _DEBUG
void CRegGridCtrl::AssertValid() const
{
	CALXGridCtrl::AssertValid();
}

void CRegGridCtrl::Dump(CDumpContext& dc) const
{
	CALXGridCtrl::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegGridCtrl message handlers


HBITMAP CRegGridCtrl::GetBitmap(int nCol, int nRow, BOOL bSelected)
{
	return m_BMP;
}

CELL_DATA CRegGridCtrl::GetCellData(int nCol, int nRow)
{
	CELL_DATA CellData = CALXGrid::GetCellData(nCol,nRow);
	if(nCol!=4)
		CellData.m_strText.Format("%u - %u",nCol,nRow);
	return CellData;
}










