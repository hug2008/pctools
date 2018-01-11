// E:\Visual Studio Project\ZeberPrint\SRC\PAGE\LablePg530CartonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include ".\LablePg530CartonDlg.h"

// SQL ����ָ��
#define CartonInsertFormat _T("INSERT INTO PcoCtBox ( Model, BoxId, Imei, Sno, OrderForm, Dt, Area ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s')")
// CLablePg530CartonDlg dialog

CLablePg530CartonDlg::CLablePg530CartonDlg(CWnd* pParent /*=NULL*/)
	: CStackPage(CLablePg530CartonDlg::IDD, pParent)
	, m_szCtboxid(_T(""))
	, m_szImei(_T(""))
	, m_szSno(_T(""))
	, m_szReprint(_T(""))
	, m_strOrder(_T(""))
	, m_strZone(_T(""))
	, m_bAg169h(FALSE)
{
	m_nCtboxidLen        = 0;

	m_nSnoLen            = 0;

	m_szImeiBarTip       = _T("");

	m_szSnoBarTip        = _T("");

	m_bXaxis             = _T("");

	m_bYaxis             = _T("");

	m_bAfont             = _T("");

	m_bWfont             = _T("");

	m_bHfont             = _T("");

	m_bBcFmt             = _T("");

	m_bBcByRa            = _T("");

	m_bBcByH             = _T("");

	m_bBcSpaceVertical   = _T("");

	m_bBcSpaceHorizontal = _T("");

	m_szZpl              = _T("");

	m_szSql              = _T("");

	m_nLenAg169h         = 0;

	m_bSortImei          = FALSE;

	m_bSortSno           = FALSE;
}

CLablePg530CartonDlg::~CLablePg530CartonDlg()
{
}

void CLablePg530CartonDlg::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_CARTON_MODEL,      m_tModel);
	DDX_Control(pDX, IDC_COMB_CARTON_MODEL,    m_cModel);
	DDX_Control(pDX, IDC_ST_CARTON_CTBOXID,    m_tCtboxid);
	DDX_Control(pDX, IDC_EDIT_CARTON_CTID,     m_eCtboxid);
	DDX_Text(pDX,    IDC_EDIT_CARTON_CTID,     m_szCtboxid);
	DDX_Control(pDX, IDC_ST_CARTON_COUNT,      m_tCount);
	DDX_Control(pDX, IDC_COMB_CARTON_QUANTITY, m_cCount);
	DDX_Control(pDX, IDC_ST_CARTON_IMEI,       m_tImei);
	DDX_Control(pDX, IDC_EDIT_CARTON_IMEI,     m_eImei);
	DDX_Text(pDX,    IDC_EDIT_CARTON_IMEI,     m_szImei);
	DDX_Control(pDX, IDC_ST_CARTON_SNO,        m_tSno);
	DDX_Control(pDX, IDC_EDIT_CARTON_SNO,      m_eSno);
	DDX_Text(pDX,    IDC_EDIT_CARTON_SNO,      m_szSno);
	DDX_Control(pDX, IDC_CARTON_LIST,          m_list);
	DDX_Control(pDX, IDC_ST_CARTON_REPRINT, m_tReprint);
	DDX_Control(pDX, IDC_EDIT_CARTON_REPRINT, m_eReprint);
	DDX_Text(pDX, IDC_EDIT_CARTON_REPRINT, m_szReprint);
	DDX_Control(pDX, IDC_EDIT_CARTON_ORDER, m_Order);
	DDX_Text(pDX, IDC_EDIT_CARTON_ORDER, m_strOrder);
	DDX_Control(pDX, IDC_ST_CARTON_ORDER, m_stOrder);
	DDX_Control(pDX, IDC_ST_CARTON_AREA, m_stZone);
	DDX_Control(pDX, IDC_EDIT_CARTON_AREA, m_eZone);
	DDX_Text(pDX, IDC_EDIT_CARTON_AREA, m_strZone);
	DDX_Control(pDX, IDC_ST_CARTON_AG169H, m_stAg169h);
	DDX_Check(pDX, IDC_CHECK_AG169H, m_bAg169h);
	DDX_Control(pDX, IDC_CHECK_AG169H, m_cAg169h);
}


BEGIN_MESSAGE_MAP(CLablePg530CartonDlg, CStackPage)
	ON_NOTIFY(NM_DBLCLK, IDC_CARTON_LIST, OnNMDblclkCartonList)
	ON_CBN_SELCHANGE(IDC_COMB_CARTON_QUANTITY, OnCbnSelchangeCombCartonQuantity)
	ON_BN_CLICKED(IDC_CHECK_AG169H, OnBnClickedCheckAg169h)
END_MESSAGE_MAP()


// CLablePg530CartonDlg message handlers
#ifndef _VK_RETURN_EXIT_
BOOL CLablePg530CartonDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)	{//������Ϣ
		if (VK_RETURN == pMsg->wParam)		{// �س���
			if (m_eImei.m_hWnd == pMsg->hwnd){
				ProcessWmsgImeiEdit(pMsg);
			}else if (m_eSno.m_hWnd == pMsg->hwnd){
				ProcessWmsgSerialNoEdit(pMsg);
			}else if (m_eReprint.m_hWnd == pMsg->hwnd){
				ProcessWmsgReprintEdit(pMsg);
			}else{
				return TRUE;
			}
			return TRUE;
		}else{// if (VK_RETURN == pMsg->wParam)	{
			return CStackPage::PreTranslateMessage(pMsg); 
		}// if (VK_RETURN == pMsg->wParam){}else{}
	}else{// if (WM_KEYDOWN == pMsg->message){
		return CStackPage::PreTranslateMessage(pMsg); 
	}// if (WM_KEYDOWN == pMsg->message){}else{}
}
#endif//_VK_RETURN_EXIT_

BOOL CLablePg530CartonDlg::OnInitDialog()
{
	int     nSel   = 0;
	CString szSel  = _T("");

	CStackPage::OnInitDialog();

	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	ASSERT(m_pParentWnd);

	m_tModel.textColor(RGB(0,0,255));
	m_tModel.setFont(-16);
	m_tModel.SetWindowText("Model:");
    /* ��AG-169H(PCO)�ų�������ѡ�������⴦�� */
	szSel = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_MODELNAME, _T("PG530@PG110@PG120"));
	if (PutStringIntoCStringArray(szSel,m_szArray)){
		FillStringIntoComboBox(&m_cModel,m_szArray);
		szSel = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_MODELSEL, _T("00"));
		nSel  = atoi(szSel);
		if (m_cModel.GetCount())
			m_cModel.SetCurSel(nSel % m_cModel.GetCount());
	}

	m_tCtboxid.textColor(RGB(0,0,255));
	m_tCtboxid.setFont(-16);
	m_tCtboxid.SetWindowText("C/T box ID:");

	szSel = theApp.m_Ini.ReadString(SEC_CARTONBOX, INENT_CARTON_CTBOXIDLEN, _T(""));
	m_nCtboxidLen = atoi(szSel);
	if (m_nCtboxidLen < 1){
		m_nCtboxidLen = 0x07;
	}
	m_eCtboxid.LimitText(m_nCtboxidLen);
	m_eCtboxid.textColor(RGB(255,0,0));
	m_eCtboxid.setFont(-16);
	m_szCtboxid = theApp.m_Ini.ReadString(SEC_CARTONBOX, INENT_CARTON_CTBOXIDLAST, _T("0700001"));
	m_eCtboxid.SetWindowText(m_szCtboxid);


	m_tCount.textColor(RGB(0,0,255));
	m_tCount.setFont(-16);
	m_tCount.SetWindowText("Quantity:");

	szSel = theApp.m_Ini.ReadString(SEC_CARTONBOX, INENT_CARTON_QUANTITY, _T(""));
	if (szSel.IsEmpty()){
		szSel = _T("01@02@03@04@05@06@07@08@09@10@11@12@13@14@15@16@17@18@19@20");
	}
	if (PutStringIntoCStringArray(szSel,m_szArray)){
		FillStringIntoComboBox(&m_cCount,m_szArray);
		szSel = theApp.m_Ini.ReadString(SEC_CARTONBOX, INENT_CARTON_QUANTITYSEL, _T("00"));
		nSel  = atoi(szSel);
		if (m_cCount.GetCount())
			m_cCount.SetCurSel(nSel % m_cCount.GetCount());
	}

	m_tImei.textColor(RGB(0,0,255));
	m_tImei.setFont(-16);
	m_tImei.SetWindowText("IMEI:");

	m_eImei.LimitText(IMEI_LENGTH);
	m_eImei.textColor(RGB(255,0,0));
	m_eImei.setFont(-16);

	m_tSno.textColor(RGB(0,0,255));
	m_tSno.setFont(-16);
	m_tSno.SetWindowText("Serial No.:");

	m_eReprint.LimitText(m_nCtboxidLen);
	m_eReprint.textColor(RGB(255,0,0));
	m_eReprint.setFont(-22);//

	m_tReprint.textColor(RGB(0,0,255));
	m_tReprint.setFont(-22);
	m_tReprint.SetWindowText("�ش���ţ�");

	szSel = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_SERIALNOLEN, _T(""));
	m_nSnoLen = atoi(szSel);
	if (m_nSnoLen < 1){
		m_nSnoLen = 10;
	}
    /* ��Ini��ȡ��ѡ��AG-169H(PCO)ʱ��Serial.No������λ��*/
	szSel = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_AG169HLEN, _T(""));
	m_nLenAg169h = atoi(szSel);
	if (m_nLenAg169h < 1){
		m_nLenAg169h = 11;
	}
	
	szSel = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_SNO_REP, _T("S/N:"));
	m_szSnoRep = szSel;

	// check box
	szSel       = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_AG169H,  _T("00"));
	m_bAg169h   = atoi(szSel);
	m_cAg169h.SetCheck(m_bAg169h);    
	m_cModel.EnableWindow(!m_bAg169h);
    /*
	if (m_bAg169h){
		m_eSno.LimitText(m_nLenAg169h + m_szSnoRep.GetLength());
	}else{
		m_eSno.LimitText(m_nSnoLen);
	}
	*/
	m_eSno.textColor(RGB(255, 0, 0));
	m_eSno.setFont(-16);


	// Imei Sort
	szSel       = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_IMEISORT,  _T("00"));
	if (atoi(szSel) > 0)
		m_bSortImei = TRUE;
	else
		m_bSortImei = FALSE;

	// Sno Sort
	szSel       = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_IMEISNO,  _T("00"));
	if (atoi(szSel) > 0)
		m_bSortSno = TRUE;
	else
		m_bSortSno = FALSE;
	

    /* (Order)�����ų�ʼ�� */
	m_strOrder = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_ORDER, _T("NC00001"));
	m_Order.LimitText(50);
	m_Order.textColor(RGB(255, 0, 0));
	m_Order.setFont(-16);
	m_Order.SetWindowText(m_strOrder);

	m_stOrder.textColor(RGB(0,0,255));
	m_stOrder.setFont(-16);
	m_stOrder.SetWindowText("Order:");

    /* ���������ʼ�� */
	m_strZone = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_ZONE, _T("��������"));
	m_eZone.LimitText(50);
	m_eZone.textColor(RGB(255, 0, 0));
	m_eZone.setFont(-16);
	m_eZone.SetWindowText(m_strZone);

	m_stZone.textColor(RGB(0,0,255));
	m_stZone.setFont(-16);
	m_stZone.SetWindowText("Area:");

	m_stAg169h.textColor(RGB(0,0,255));
	m_stAg169h.setFont(-16);
	m_stAg169h.SetWindowText("AG-169H(PCO):");

	m_szImeiBarTip = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_IMEIBARCODETIP, _T("IMEI:"));
	m_szSnoBarTip  = theApp.m_Ini.ReadString(SEC_CARTONBOX, IDENT_CARTON_SNOBARCODETIP,  _T("S/NO:"));

	// ��ӡ��ʽ��ʼ��
	m_bXaxis  = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_AXIS_X,  _T("80"));
	m_bYaxis  = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_AXIS_Y,  _T("80"));
	m_bAfont  = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_FONT_A,  _T("^A0N,%d,%d"));
	m_bWfont  = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_FONT_W,  _T("60"));
	m_bHfont  = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_FONT_H,  _T("60"));
	m_bBcFmt  = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_BARCODE, _T("^FO%d,%d^B3N,N,,N,N^FD%s^FS"));
	m_bBcByRa = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_BCBY_RA, _T("^BY2,2,%d"));
	m_bBcByH  = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_BCBY_H,  _T("70"));
	m_bBcSpaceVertical = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_DIS_VTC,  _T("20"));
	m_bBcSpaceHorizontal = theApp.m_Ini.ReadString(SEC_CARTONBOX, ZPL_PG_CATON_DIS_HZT,  _T("600"));


	// list
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_list.EnableToolTips(TRUE);

	CRect rect;

	TCHAR *lpszHeaders[] = {
		    _T("Index"),
			_T("IMEI"),
			_T("Serial No.")
	};

	m_list.GetWindowRect(&rect);

	int w = rect.Width();

	w = w - ::GetSystemMetrics(SM_CXVSCROLL) - 2;	// width of vertical scroll bar

	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	lvcolumn.pszText = lpszHeaders[0];
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = 45;
	m_list.InsertColumn(0, &lvcolumn);

	lvcolumn.pszText = lpszHeaders[1];
	lvcolumn.iSubItem = 1;
	lvcolumn.cx = w - lvcolumn.cx - 120;
	m_list.InsertColumn(1, &lvcolumn);

	lvcolumn.pszText = lpszHeaders[2];
	lvcolumn.iSubItem = 2;
	lvcolumn.cx = 120;
	m_list.InsertColumn(2, &lvcolumn);
	m_list.DeleteAllItems();



	// ռ���λ��
	OnSaveData();

	return TRUE;
}

void CLablePg530CartonDlg::OnSaveData(void)
{
	CStackPage::UpdateData(TRUE);

	CString szVal = _T("");
    /* �����б��� */
	GetComboBoxItemToStringArray(m_szArray, &m_cModel);
	PrintfCStringArrayToString(szVal, m_szArray);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_MODELNAME, szVal);
    /* ��ǰ�б����� */
	szVal.Format("%02d", m_cModel.GetCurSel());
	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_MODELSEL, szVal);

	szVal.Format("%02d", m_nCtboxidLen);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, INENT_CARTON_CTBOXIDLEN, szVal);

	theApp.m_Ini.WriteString(SEC_CARTONBOX, INENT_CARTON_CTBOXIDLAST, m_szCtboxid);
    /* ���� */ 
	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_ORDER, m_strOrder);
    /* �������� */
	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_ZONE, m_strZone);

    /* S.no ȥ����ǰ׺ */
    theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_SNO_REP, m_szSnoRep);

	// check box
	szVal.Format("%02d", m_cAg169h.GetCheck());
	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_AG169H, szVal);

	// Imei Sort
	szVal.Format("%02d", m_bSortImei);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_IMEISORT, szVal);

	// Sno Sort
	szVal.Format("%02d", m_bSortSno);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_IMEISNO, szVal);

	GetComboBoxItemToStringArray(m_szArray, &m_cCount);
	PrintfCStringArrayToString(szVal, m_szArray);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, INENT_CARTON_QUANTITY, szVal);

	szVal.Format("%02d", m_cCount.GetCurSel());
	theApp.m_Ini.WriteString(SEC_CARTONBOX, INENT_CARTON_QUANTITYSEL, szVal);

	szVal.Format("%02d", m_nSnoLen);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_SERIALNOLEN, szVal);

	szVal.Format("%02d", m_nLenAg169h);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_AG169HLEN, szVal);

	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_IMEIBARCODETIP, m_szImeiBarTip);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, IDENT_CARTON_SNOBARCODETIP,  m_szSnoBarTip);

	theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_AXIS_X,  m_bXaxis);
    theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_AXIS_Y,  m_bYaxis);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_FONT_A,  m_bAfont);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_FONT_W,  m_bWfont);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_FONT_H,  m_bHfont);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_BARCODE, m_bBcFmt);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_BCBY_RA, m_bBcByRa);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_BCBY_H,  m_bBcByH);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_DIS_VTC, m_bBcSpaceVertical);
	theApp.m_Ini.WriteString(SEC_CARTONBOX, ZPL_PG_CATON_DIS_HZT,  m_bBcSpaceHorizontal);	
}

/* �������� */
void CLablePg530CartonDlg::OnLockPage(BOOL bLock)
{
	m_cModel.EnableWindow(!bLock);
	m_cCount.EnableWindow(!bLock);
	m_Order.EnableWindow(!bLock);
	m_eZone.EnableWindow(!bLock);
	m_cAg169h.EnableWindow(!bLock);
}

BOOL CLablePg530CartonDlg::CheckMeSeting(void)
{// 
	CStringArray szBoxidArray;

	m_eCtboxid.GetWindowText(m_szCtboxid);

	if (!IsEmptyOrNumberString(m_szCtboxid)){

		theApp.m_szStatus.Format(_T("Box ID[%s] ֻ�ܹ�������!"), m_szCtboxid);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_eCtboxid.SetFocus();

		m_eCtboxid.SetSel(0, -1);

		return FALSE;
	}

	if (m_szCtboxid.GetLength() != m_nCtboxidLen){

		theApp.m_szStatus.Format(_T("Box ID[%s] ֻ�ܹ��� %d �ַ�����!"), m_szCtboxid, m_nCtboxidLen);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_eCtboxid.SetFocus();

		m_eCtboxid.SetSel(0, -1);

		return FALSE;
	}

	theApp.m_Ini.ReadSection(m_szCtboxid, szBoxidArray);

	if (szBoxidArray.GetSize()){

		theApp.m_szStatus.Format(_T("Carton Box ID[%s] �Ѿ�ʹ�ã�\r\n�Ƿ������ӡ��"), m_szCtboxid);

		if (IDYES != MessageBox(theApp.m_szStatus, _T("Zaber Printer"), MB_YESNO|MB_ICONASTERISK)){
			return FALSE;
		}
	}

	// �ڴ˻�ȡ����
	m_cCount.GetWindowText(m_szCount);

	return TRUE;
}

BOOL CLablePg530CartonDlg::PrintMeLable(void)
{
	CString  szZpl      = _T("");

	CString  szFontFmt  = _T(""),
		     szBcodeFmt = _T(""),
			 szBcodeStr = _T("");

	int nXaxis          = atoi(m_bXaxis),
		nYaxis          = atoi(m_bYaxis);

	int nFontW          = atoi(m_bWfont),
		nFontH          = atoi(m_bHfont);

	int nSel            = m_cModel.GetCurSel();

	m_szZpl.Empty();

	// ��ӡ��ʽ����
	szFontFmt += _T("^FO%d,%d");
	szFontFmt += m_bAfont;
	szFontFmt += _T("^FH\^FD%s^FS");

	// ��ʼ��λ
	szZpl.Format(_T("^XA^FO%d,%d"), nXaxis, nYaxis);
	m_szZpl   += szZpl;
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "[SerialNo:%s]\r", m_szCtboxid);
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// �ͺš���ͨ���˵��
	m_cModel.GetLBText(nSel, szBcodeFmt);
#if !defined(UCS2_ENCODE)
	szBcodeStr.Format(_T("Model:%s   C/T box ID:%s"), szBcodeFmt, m_szCtboxid);
#else
	if (0 == theApp.m_nMmiIndex){
		szBcodeStr.Format(_T("Model:%s   C/T box ID:%s"), szBcodeFmt, m_szCtboxid);
	}else{
		szBcodeStr.Format(_T("Modelo:%s   ID Caja C/T:%s"), szBcodeFmt, m_szCtboxid);
	}
#endif
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, szBcodeStr);
	if (!m_bAg169h){// AG-169H(PCO) �Ͳ�Ҫ��ӡ������
		m_szZpl   += szZpl;
	}
	
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// �����ӡ��ʽ����
	szBcodeFmt.Format(_T("%s%s"), m_bBcByRa, m_bBcFmt);

	// ����������
	m_szCount.Format("%d", atoi(m_szCount));

#if !defined(UCS2_ENCODE)
	szZpl.Format(szBcodeFmt, nFontH-10, (nXaxis + atoi(m_bBcSpaceHorizontal) + 350), nYaxis, m_szCount);
#else
	if (0 == theApp.m_nMmiIndex){
		szZpl.Format(szBcodeFmt, nFontH-10, (nXaxis + atoi(m_bBcSpaceHorizontal) + 350), nYaxis, m_szCount);
	}else{
		szZpl.Format(szBcodeFmt, nFontH-10, (nXaxis + atoi(m_bBcSpaceHorizontal) + 380), nYaxis, m_szCount);
	}
#endif

	if (!m_bAg169h){// AG-169H(PCO) �Ͳ�Ҫ������������
		m_szZpl   += szZpl;
	}

#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// �ƶ�һ��x��y������
	nXaxis += 60;
	if (m_bAg169h){
		nYaxis += 70;
	}else{
		nYaxis = nYaxis + nFontH + 70;
	}

	// �ֺ���С
	nFontH /= 2;
	nFontW /= 2;

	// ��Դ�ӡ����
	for (nSel=0; nSel<min(m_szImeiArray.GetSize(), m_szSnoArray.GetSize()); nSel++){

		// IMEI ������
		szZpl.Format(szBcodeFmt, 
			atoi(m_bBcByH), nXaxis, nYaxis, m_szImeiArray.GetAt(nSel));		
		m_szZpl   += szZpl;

#ifdef SEC_ZPL_LOG
		if (theApp.m_fLog){
			fprintf(theApp.m_fLog, "%s\r",   szZpl);
		}
#endif//SEC_ZPL_LOGszFontFmt

		// ��ϸ��Imei��
		if (m_szImeiBarTip.GetLength()){

			szBcodeStr.Format("%s%s", m_szImeiBarTip, m_szImeiArray.GetAt(nSel));

			szZpl.Format(szFontFmt, nXaxis, nYaxis + atoi(m_bBcByH) + 15, nFontH, nFontW, szBcodeStr);

			m_szZpl   += szZpl;

#ifdef SEC_ZPL_LOG
			if (theApp.m_fLog){
				fprintf(theApp.m_fLog, "%s\r",   szZpl);
			}
#endif//SEC_ZPL_LOGszFontFmt
		}//if (m_szImeiBarTip.GetLength()){}



		// Serial No. ������
		szZpl.Format(szBcodeFmt,
			atoi(m_bBcByH), nXaxis + atoi(m_bBcSpaceHorizontal), nYaxis, m_szSnoArray.GetAt(nSel));		
		m_szZpl   += szZpl;
#ifdef SEC_ZPL_LOG
		if (theApp.m_fLog){
			fprintf(theApp.m_fLog, "%s\r",   szZpl);
		}
#endif//SEC_ZPL_LOGszFontFmt

			// ��ϸ��Serial NO.��
		if (m_szSnoBarTip.GetLength()){
#if !defined(UCS2_ENCODE)
			szBcodeStr.Format("%s%s", m_szSnoBarTip, m_szSnoArray.GetAt(nSel));
#else
			if (0 == theApp.m_nMmiIndex){
				szBcodeStr.Format("%s%s", m_szSnoBarTip, m_szSnoArray.GetAt(nSel));
			}else{
				szBcodeStr.Format("%s%s", _T("S/Nro:"), m_szSnoArray.GetAt(nSel));
			}
#endif//UCS2_ENCODE

			szZpl.Format(szFontFmt, 
				(nXaxis+atoi(m_bBcSpaceHorizontal)), (nYaxis+atoi(m_bBcByH)+15), nFontH, nFontW, szBcodeStr);

			m_szZpl   += szZpl;

#ifdef SEC_ZPL_LOG
			if (theApp.m_fLog){
				fprintf(theApp.m_fLog, "%s\r",   szZpl);
			}
#endif//SEC_ZPL_LOGszFontFmt
		}//if (m_szImeiBarTip.GetLength()){}

		// y ��������
		nYaxis = nYaxis + atoi(m_bBcByH) + 15 + atoi(m_bBcSpaceVertical);
		if (m_szImeiBarTip.GetLength()| m_szSnoBarTip.GetLength()){
			nYaxis += nFontH;
		}

	}//for (nSel=0; nSel<min(m_szImeiArray.GetSize(), m_szSnoArray.GetSize()); nSel++){}

	// ��ӡ����
	szZpl.Format(_T("^PQ%d,0,1,Y"), atoi(theApp.m_szShare));
	m_szZpl += szZpl;

	// ��ӡ����
	m_szZpl += _T("^XZ");
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r^XZ\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	return (ExecutePrintCmdInLpt(m_szZpl, theApp.m_szLpt) == SUCCEED_LPT);
}

void CLablePg530CartonDlg::UpDateMeAfterPrint(void)
{
	char szTemp[32] = "";

	m_eCtboxid.GetWindowText(m_szCtboxid);

	GetlistData(m_szImeiArray, m_szSnoArray);
    /* �����б�ؼ����ݵ����ݿ⣬���Ҹ�֪User */
	SaveListDataToMdb(m_szImeiArray, m_szSnoArray);

	theApp.m_szStatus.Format(_T("��ӡ��ͨ��[%s]��ǩ����,�����ѱ���"), m_szCtboxid);

	::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, TRUE, NULL);

	SaveListData(m_szImeiArray, m_szSnoArray);

	strcpy(szTemp, m_szCtboxid);

	NumStrAutoAdd(szTemp);

	m_szCtboxid = szTemp;

	m_eCtboxid.SetWindowText(m_szCtboxid);

	m_list.DeleteAllItems();

	m_szImeiArray.RemoveAll();
	m_szSnoArray.RemoveAll();

	m_szSno.Empty();
	m_eSno.SetWindowText(m_szSno);

	m_szImei.Empty();
	m_eImei.SetWindowText(m_szImei);
	m_eImei.SetFocus();
	m_eImei.SetSel(0, -1);

	return;
}

void CLablePg530CartonDlg::GetlistData(CStringArray &szImeiArray, CStringArray &szSnoArray)
{
	int i = 0, j = 0;

	CString szTemp = _T("");

	szImeiArray.RemoveAll();

	szSnoArray.RemoveAll();

	for (i=0; i<m_list.GetItemCount(); i++){

		szImeiArray.Add(m_list.GetItemText(i, 0x01));

		szSnoArray.Add(m_list.GetItemText(i, 0x02));
	}

	if (m_bSortImei){// Imei���ȣ���С��������
		for (i=0; i<szImeiArray.GetSize(); i++)
			for (j=i; j<szImeiArray.GetSize() - 1; j++)
			{
				if (strcmp(szImeiArray[j], szImeiArray[j+1]) > 0)
				{
					// Imei �Ŵ�С����
					szTemp           = szImeiArray[j];
					szImeiArray[j]   = szImeiArray[j+1];
					szImeiArray[j+1] = szTemp;
                    
					// ���к�Ҳ���ŵ���
					szTemp          = szSnoArray[j];
					szSnoArray[j]   = szSnoArray[j+1];
					szSnoArray[j+1] = szTemp;
				}
			}
	}// Imei���ȣ���С�������� 

	if (m_bSortSno){// Sno���ȣ���С��������
		for (i=0; i<szSnoArray.GetSize(); i++)
			for (j=i; j<szSnoArray.GetSize() - 1; j++)
			{
				if (strcmp(szSnoArray[j], szSnoArray[j+1]) > 0)
				{
					// ���к�Ҳ���ŵ���
					szTemp           = szSnoArray[j];
					szSnoArray[j]    = szSnoArray[j+1];
					szSnoArray[j+1]  = szTemp;
                    
					// Imei �Ŵ�С����
					szTemp           = szImeiArray[j];
					szImeiArray[j]   = szImeiArray[j+1];
					szImeiArray[j+1] = szTemp;
				}
			}
	}// Sno���ȣ���С��������

	return;
}

void CLablePg530CartonDlg::SaveListData(CStringArray &szImeiArray, CStringArray &szSnoArray)
{
	GetlistData(szImeiArray, szSnoArray);

	for (int i=0; i<szSnoArray.GetSize(); i++)

		theApp.m_Ini.WriteString(m_szCtboxid, szImeiArray.GetAt(i), szSnoArray.GetAt(i));

	return;
}

void CLablePg530CartonDlg::RefreshList(void)
{
	CString szIndex = _T("");

	for (int i=0; i<m_list.GetItemCount(); i++){

		szIndex.Format("%02d",  i + 1);

		m_list.SetItemText(i, 0, szIndex);
	}

	return;
}

int CLablePg530CartonDlg::InsertItemToList(int nItemIndex, const char* szImei, const char* szSno)
{
	CString szCount = _T("");

	int nItemCount  = m_list.GetItemCount();

	szCount.Format(_T("%02d"), nItemCount + 1);

	m_list.InsertItem(nItemCount, szCount);

	m_list.SetItemText(nItemCount, 0x01, szImei);

	m_list.SetItemText(nItemCount, 0x02, szSno);

	return (nItemCount + 1);
}

void CLablePg530CartonDlg::ProcessWmsgImeiEdit(MSG* pImeiMsg)
{
	CString szCount = _T("");

	CString  szSno  = _T("");

	CString  szModel = _T("");

	m_cCount.GetWindowText(szCount);

	int nCount      = atoi(szCount);

	int nItemCount  = m_list.GetItemCount();

	long nOccurIndex = -1;

	if (VK_RETURN != pImeiMsg->wParam){
		return;
	}

	// Check this Imei exist in list
	m_eImei.GetWindowText(m_szImei);

	if (!IsEmptyOrHexString(m_szImei)){

		theApp.m_szStatus.Format(_T("Imei[%s] ֻ�ܹ��� 16 ������!"), m_szImei);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return;
	}

	if (m_szImei.GetLength() != IMEI_LENGTH){

		theApp.m_szStatus.Format(_T("Imei[%s] ֻ�ܹ��� %d �ַ�����!"), m_szImei, IMEI_LENGTH);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return;
	}

	// Check this Imei exist wether in mdb
	nOccurIndex = GetItemIndexInMdb(_T("Imei"), m_szImei);
	if (nOccurIndex > 0){

		theApp.m_szStatus.Format(_T("Imei[%s] ���ݿ�PcoCtBox�Ѵ���[Serial=%d]!"), m_szImei, nOccurIndex);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return;
	}
    // ���ٴ�Ini�����ѯ��ص�Serial No.�����û�ֱ��ɨ��
	// Verify Imei Iterance in list
	nOccurIndex = IsItemOccurInList(&m_list, 0x01, m_szImei);

	if (-1 < nOccurIndex){

		theApp.m_szStatus.Format(_T("Imei[%s] �Ѿ����б��[%d]����¼��!"), m_szImei, nOccurIndex+1);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return;
	}

	// ��ת�� Serial No. Edit
	m_eSno.SetFocus();
	m_eSno.SetSel(0, -1);

	return;
}

void CLablePg530CartonDlg::ProcessWmsgSerialNoEdit(MSG* pSnoMsg)
{
	CString szCount = _T("");

	m_cCount.GetWindowText(szCount);

	int nCount      = atoi(szCount);

	int nItemList   = m_list.GetItemCount();

	long nOccurIndex = -1;

	if (VK_RETURN != pSnoMsg->wParam){
		return;
	}

	// Check this Imei
	m_eImei.GetWindowText(m_szImei);
	if (!IsEmptyOrHexString(m_szImei)){

		theApp.m_szStatus.Format(_T("Imei[%s] ֻ�ܹ���16�����ַ�!"), m_szImei);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return;
	}

	if (m_szImei.GetLength() != IMEI_LENGTH){

		theApp.m_szStatus.Format(_T("Imei[%s] ֻ�ܹ��� %d �ַ�����!"), m_szImei, IMEI_LENGTH);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return;
	}

	// Check this Imei exist wether in mdb
	nOccurIndex = GetItemIndexInMdb(_T("Imei"), m_szImei);
	if (nOccurIndex > 0){

		theApp.m_szStatus.Format(_T("Imei[%s] ���ݿ�PcoCtBox�Ѵ���[Serial=%d]!"), m_szImei, nOccurIndex);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return;
	}

	// Verify Imei Iterance in list
	nOccurIndex = IsItemOccurInList(&m_list, 0x01, m_szImei);

	if (-1 < nOccurIndex){

		theApp.m_szStatus.Format(_T("Imei[%s] �Ѿ����б��[%d]����¼��!"), m_szImei, nOccurIndex+1);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return;
	} 

	// Check this Serial No.
	m_eSno.GetWindowText(m_szSno);
	m_szSno.Replace(m_szSnoRep, _T(""));
	m_eSno.SetWindowText(m_szSno);

	if (!IsHexString(m_szSno)){
		theApp.m_szStatus.Format(_T("Serial No.[%s] ֻ�ܹ��� 16 �����ַ�!"), m_szSno);
		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);
		m_szSno.Empty();
		m_eSno.SetWindowText(m_szSno);
		m_eSno.SetFocus();
		m_eSno.SetSel(0, -1);
		return;
	}
	/// AG-169H �����к�ֻ�ܹ�������
	if (m_bAg169h){
		if (!IsNumberString(m_szSno)){
			theApp.m_szStatus.Format(_T("AG-169H �� Serial No.[%s] ֻ�ܹ�������!"), m_szSno);
			::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);
			m_szSno.Empty();
			m_eSno.SetWindowText(m_szSno);
			m_eSno.SetFocus();
			m_eSno.SetSel(0, -1);
			return;
		}

		if (m_szSno.GetLength() != m_nLenAg169h){
			theApp.m_szStatus.Format(_T("AG-169h Serial No.[%s] ֻ�ܹ��� %d �ַ�����!"), m_szSno, m_nLenAg169h);
			::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);
			m_szSno.Empty();
			m_eSno.SetWindowText(m_szSno);
			m_eSno.SetFocus();
			m_eSno .SetSel(0, -1);
			return;
		}
	}else{
		if (m_szSno.GetLength() != m_nSnoLen){
			theApp.m_szStatus.Format(_T("Serial No.[%s] ֻ�ܹ��� %d �ַ�����!"), m_szSno, m_nSnoLen);
			::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);
			m_szSno.Empty();
			m_eSno.SetWindowText(m_szSno);
			m_eSno.SetFocus();
			m_eSno .SetSel(0, -1);
			return;
		}
	}	

	// Check this Serial No. exist wether in  mdb
	nOccurIndex = GetItemIndexInMdb(_T("Sno"), m_szSno);
	if (nOccurIndex > 0){

		theApp.m_szStatus.Format(_T("Serial No.[%s] ���ݿ�PcoCtBox�Ѵ���(Serial=%d)"), m_szSno, nOccurIndex);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szSno.Empty();

		m_eSno.SetWindowText(m_szSno);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return;
	}

	// Verify Imei Iterance in list
	nOccurIndex = IsItemOccurInList(&m_list, 0x02, m_szSno);

	if (-1 < nOccurIndex){

		theApp.m_szStatus.Format(_T("Serial No.[%s] �Ѿ����б��[%d]����¼��!"), m_szSno, nOccurIndex+1);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szSno.Empty();

		m_eSno.SetWindowText(m_szSno);

		m_eSno.SetFocus();

		m_eSno.SetSel(0, -1);

		return;
	}

	// AddItem to list
	nItemList = InsertItemToList(nItemList, m_szImei, m_szSno);

	if (nItemList < nCount){

		m_szSno.Empty();

		m_eSno.SetWindowText(m_szSno);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return;
	}

	if (CheckMeSeting()){
		GetlistData(m_szImeiArray, m_szSnoArray);		
		if (PrintMeLable()){
			UpDateMeAfterPrint();
		}else{
			m_eCtboxid.GetWindowText(m_szCtboxid);
			theApp.m_szStatus.Format(_T("��ӡ��ͨ��[%s]��ǩʧ��,����δ����"), m_szCtboxid);
			::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);
			m_list.DeleteAllItems();
		}
	}

	return;
}

void CLablePg530CartonDlg::ProcessWmsgReprintEdit(MSG* pReprintMsg)
{
	CString  szMsg = _T("");

	int nItem      = 0;

	if (VK_RETURN != pReprintMsg->wParam){
		return;
	}

	m_eReprint.GetWindowText(m_szReprint);

	if (!IsNumberString(m_szReprint)){

		theApp.m_szStatus.Format(_T("��ͨ���[%s]����������"), m_szReprint);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szReprint.Empty();

		m_eReprint.SetWindowText(m_szReprint);

		m_eReprint.SetFocus();

		m_eReprint.SetSel(0, -1);

		return;
	}

	if (m_szReprint.GetLength() != m_nCtboxidLen){

		theApp.m_szStatus.Format(_T("��ͨ���[%s]������ %d �ַ�����"), m_szReprint, m_nCtboxidLen);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szReprint.Empty();

		m_eReprint.SetWindowText(m_szReprint);

		m_eReprint.SetFocus();

		m_eReprint.SetSel(0, -1);

		return;
	}

	m_szImeiArray.RemoveAll();
	m_szSnoArray.RemoveAll();
	if (!theApp.m_Ini.ReadSection(m_szReprint, m_szImeiArray))
	{
		theApp.m_szStatus.Format(_T("�޷���ѯ����ͨ���[%s]�ļ�¼!"), m_szReprint);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szReprint.Empty();

		m_eReprint.SetWindowText(m_szReprint);

		m_eReprint.SetFocus();

		m_eReprint.SetSel(0, -1);

		return;
	}

	if (!m_szImeiArray.GetSize())
	{
		theApp.m_szStatus.Format(_T("�޷���ѯ����ͨ���[%s]�ļ�¼!"), m_szReprint);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szReprint.Empty();

		m_eReprint.SetWindowText(m_szReprint);

		m_eReprint.SetFocus();

		m_eReprint.SetSel(0, -1);

		return;
	}

	// ȡ����Ӧ��Serial NO
	for (nItem=0; nItem<m_szImeiArray.GetSize(); nItem++)
		m_szSnoArray.Add(theApp.m_Ini.ReadString(m_szReprint, m_szImeiArray.GetAt(nItem), _T("")));

	// �������������б�
	m_szCount.Format("%02d", m_szSnoArray.GetSize());
	m_cCount.SetWindowText(m_szCount);

	szMsg       = m_szCtboxid;
	m_szCtboxid = m_szReprint;
	// ��ӡ
	PrintMeLable();

	m_szCtboxid = szMsg;

	m_eCtboxid.SetWindowText(m_szCtboxid);

	m_szReprint.Empty();

	m_eReprint.SetWindowText(m_szReprint);

	m_eReprint.SetFocus();

	m_eReprint.SetSel(0, -1);

	return;
}

void CLablePg530CartonDlg::OnNMDblclkCartonList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int nItem = pNMLV->iItem;

	CString szMsg = _T("");

	szMsg.Format(_T("ɾ���� [%02d] ����¼��"), nItem+1);

	if (nItem < 0){
		return;
	}

	if (IDYES == MessageBox(szMsg, _T("Zaber Printer"), MB_YESNO|MB_ICONWARNING)){
		m_list.DeleteItem(nItem);
		RefreshList();
	}

	*pResult = 0;
}

int IsItemOccurInList(CListCtrl* pListCtrl, int nCol, const char* szItemText)
{
	CString szItem = _T("");

	if (0 == pListCtrl->GetItemCount()){
		return (-1);
	}

	for (int i=0; i<pListCtrl->GetItemCount(); i++){

		szItem = pListCtrl->GetItemText(i, nCol);

		if (szItem == szItemText){

			pListCtrl->SetFocus();

			pListCtrl->SetSelectionMark(i);

			return (i);
		
		}			
	}

	return (-1);
}

void CLablePg530CartonDlg::OnCbnSelchangeCombCartonQuantity()
{
	// TODO: Add your control notification handler code here
	CStackPage::UpdateData(TRUE);
}

long CLablePg530CartonDlg::GetItemIndexInMdb(const char* szFieldName, const char *szVal)
{
	CString     szSqlVal = _T("");
	CString     szSqlCmd = _T("");
	szSqlCmd.Format(_T("SELECT Serial FROM PcoCtBox WHERE %s=\"%s\""), szFieldName, szVal);
	theApp.m_ado.ExecuteQueryValue(szSqlCmd, szSqlVal);
	return atol(szSqlVal);
}

void CLablePg530CartonDlg::SaveListDataToMdb(CStringArray &szImeiArray, CStringArray &szSnoArray)
{
	CStackPage::UpdateData(TRUE);
	CString  szModel = _T("");
	SYSTEMTIME curTime;
	GetLocalTime( &curTime );
	CString szTime;
	szTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear,curTime.wMonth,curTime.wDay,
		                                     curTime.wHour,curTime.wMinute,curTime.wSecond);
	if (m_bAg169h){
		szModel = _T("AG-169H(PCO)");
	}else{
		m_cModel.GetWindowText(szModel);
	}
	
	m_szSql.Empty();
	theApp.m_ado.BeginTrans();
	for (int index=0; index<min(szImeiArray.GetSize(), szSnoArray.GetSize()); index++)
	{ //( Model, BoxId, Imei, Sno, OrderForm, Dt, Area )
		m_szSql.Format(CartonInsertFormat, 
			szModel, m_szCtboxid, szImeiArray.GetAt(index), szSnoArray.GetAt(index), m_strOrder, szTime, m_strZone);
		theApp.m_ado.Execute(m_szSql);
	}
	theApp.m_ado.CommitTrans();
}

void CLablePg530CartonDlg::OnBnClickedCheckAg169h()
{
	// TODO: Add your control notification handler code here
	m_bAg169h = m_cAg169h.GetCheck();
	m_cModel.EnableWindow(!m_bAg169h);
	if (m_bAg169h){
		m_szSno = m_szSno.Left(m_nLenAg169h + m_szSnoRep.GetLength());
		m_eSno.SetWindowText(m_szSno);
		m_eSno.LimitText(m_nLenAg169h + m_szSnoRep.GetLength());	
	}else{
		m_eSno.LimitText(m_nSnoLen);
		m_eSno.SetWindowText(m_szSno);
		m_szSno = m_szSno.Left(m_nSnoLen);
	}
}
