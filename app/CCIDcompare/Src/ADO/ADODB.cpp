#include "stdafx.h"
#include "ADODB.h"

CAdoDB::CAdoDB()
{
	// ��ʼ��COM,����ADO���ӵȲ���
	::AfxOleInit();
	//����CoInitialize��ʼ��COM����
	::CoInitialize(NULL);
	m_pConn.CreateInstance(__uuidof(Connection));
	m_pRs.CreateInstance(__uuidof(Recordset));
}

CAdoDB::~CAdoDB()
{
	//����CoUninitialize�ͷ�COM����
	::CoUninitialize();
}

BOOL CAdoDB::IsOpen()
{
	try
	{
		return ((m_pConn != NULL) && (m_pConn->State & adStateOpen));
	}
	catch(_com_error e)//�쳣����
	{
		AfxMessageBox(e.ErrorMessage());
	}
	return FALSE;
}

_RecordsetPtr CAdoDB::Execute(LPCTSTR lpszSQL, long lOptions)//adCmdText
{
	ASSERT(m_pConn != NULL);
	ASSERT(AfxIsValidString(lpszSQL));
	_variant_t  RecordsAffected;
	if (!IsOpen()){
		MessageBox(NULL, _T("���ݿ�δ�򿪣�ִ��SQL����ʧ�ܣ�"), _T("��ʾ"), MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	try
	{//ִ��sql���
		return m_pConn->Execute(_bstr_t(lpszSQL), &RecordsAffected, lOptions);
		//return m_pConn->Execute(_bstr_t(lpszSQL), NULL, lOptions);
	}
	catch(_com_error e)//�쳣����
	{
	    MessageBox(NULL, e.ErrorMessage(), _T("���ݿ��쳣"),MB_OK|MB_ICONSTOP);
		MessageBox(NULL, e.Source(), _T("���ݿ��쳣"),MB_OK|MB_ICONSTOP);
		MessageBox(NULL, e.Description(), _T("���ݿ��쳣"),MB_OK|MB_ICONSTOP);
	}
	return FALSE;
}

BOOL CAdoDB::Open(LPCTSTR lpszConnect, long lOptions)
{
	//����Connection����
	//m_pConn.CreateInstance(_uuidof(Connection));
	//��ʼ��Recordsetָ��
	//m_pRs.CreateInstance(_uuidof(Recordset));
	m_strConn = lpszConnect;
	if (m_strConn.IsEmpty())
	{
		return FALSE;
	}
	if (IsOpen()) Close();
	try
	{
		//���ݿ�����
		return (m_pConn->Open(_bstr_t(m_strConn), "", "", lOptions) == S_OK);
	}
	catch(_com_error e)//�쳣����
	{
		AfxMessageBox(e.ErrorMessage());
		MessageBox(NULL,"�����ݿ�ʧ�ܣ�\r�������ݿ��Ƿ���ָ��Ŀ¼��!","����",MB_OK);
	}
	return FALSE;
}

void  CAdoDB::Close()
{
	try
	{
		if ((m_pConn != NULL) && IsOpen())
		{
			//�ر����ݿ�����
			m_pConn->Close();
		}
	}
	catch(_com_error e)//�쳣����
	{
		AfxMessageBox(e.ErrorMessage());
	}
}

void  CAdoDB::ExecuteQueryValue(LPCTSTR lpszSQL, CString& value)
{
	try
	{
		//�򿪼�¼��
		m_pRs->Open(_bstr_t(lpszSQL), 
		            (IDispatch*)m_pConn, 
					adOpenDynamic,
					adLockOptimistic,
					adCmdText);
	    if (!m_pRs->adoEOF)
		{
			//��ȡ�ֶ�ֵ
           _variant_t  thevalue = m_pRs->GetCollect(_variant_t((short)0));
		   value = vartostr(thevalue);
		}
		m_pRs->Close();
	}
	catch(_com_error e)//�쳣����
	{
		AfxMessageBox(e.ErrorMessage());
	}
}

void  CAdoDB::ExecuteQuery(LPCTSTR lpszSQL, CDStrs& Fields)
{
	try
	{
		//�򿪼�¼��
		m_pRs->Open(_bstr_t(lpszSQL), 
		            (IDispatch*)m_pConn, 
					adOpenDynamic,
					adLockOptimistic,
					adCmdText);
		while(!m_pRs->adoEOF)
		{
			CStrs strs;
			_variant_t thevalue;
			//��ȡһ����¼�������ֶ�ֵ���浽�ַ�������strs��
			int size = m_pRs->Fields->Count;
			for (int index = 0; index < size; index++){
				thevalue = m_pRs->GetCollect(_variant_t(index));
				CString temp = vartostr(thevalue);
				strs.push_back(temp);
			}
			//����һ����¼����ά������
			Fields.push_back(strs);
			//ת����һ����¼
			m_pRs->MoveNext();
		}
		m_pRs->Close();
	}
	catch(_com_error e)//�쳣����
	{
		AfxMessageBox(e.ErrorMessage());
	}
}

CString CAdoDB::vartostr(_variant_t& var)
{
	CString strValue;
	switch(var.vt)
	{
	//�ַ���
	case VT_BSTR:
	case VT_LPSTR:
	case VT_LPWSTR:
		strValue = (LPCTSTR) (_bstr_t)var;
		break;
	//BYTE����
	case VT_I1:
	case VT_UI1:
		strValue.Format("%d", var.bVal);
		break;
	//������
	case VT_I2:
		strValue.Format("%d", var.iVal);
		break;
	//�޷��Ŷ�����
	case VT_UI2:
		strValue.Format("%d", var.uiVal);
		break;
	//����
	case VT_INT:
		strValue.Format("%d", var.intVal);
		break;
	//����
	case VT_I4:
		strValue.Format("%d", var.lVal);
		break;
	//������
	case VT_I8:
		strValue.Format("%d", var.lVal);
		break;
	//�޷�������
	case VT_UINT:
		strValue.Format("%d", var.uintVal);
		break;
	//�޷�������
	case VT_UI4:
		strValue.Format("%d", var.ulVal);
		break;
	//�޷��ų�����
	case VT_UI8:
		strValue.Format("%d", var.ulVal);
		break;
	case VT_VOID:
		strValue.Format("%8x", var.byref);
		break;
	//�����ͣ�������λС��
	case VT_R4:
		strValue.Format("%.2f", var.fltVal);
		break;
	//˫�����ͣ�������λС��
	case VT_R8:
		strValue.Format("%.2f", var.dblVal);
		break;
	//��ֵ�ͣ�������λС��
	case VT_DECIMAL:
		strValue.Format("%.2f", (double)var);
		break;
	//������
	case VT_CY:
		{
			COleCurrency cy = var.cyVal;
			strValue = cy.Format();
	    }
		break;
	//blob������
	case VT_BLOB:
	case VT_BLOB_OBJECT:
	case 0x2011:
		strValue = "[BLOB]";
		break;
	//������
    case VT_BOOL:
		strValue = var.boolVal ? "TRUE" : "FALSE";
		break;
	//������
	case VT_DATE:
		{
			DATE dt = var.date;
			COleDateTime oleDt = COleDateTime(dt);
			strValue = oleDt.Format("%Y-%m-%d %H:%M:%S");
	    }
		break;
	//NULL��
	case VT_NULL:
		strValue = "NULL";
		break;
	case VT_EMPTY:
		strValue = "";
		break;
	//δ֪���ͻ�����������
	case VT_UNKNOWN:
	default:
	    strValue = "UN_KNOW";
		break;
	}
	return strValue;
}