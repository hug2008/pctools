#include "stdafx.h"
#include "ADODB.h"

CAdoDB::CAdoDB()
{
	// 初始化COM,创建ADO连接等操作
	::AfxOleInit();
	//调用CoInitialize初始化COM环境
	::CoInitialize(NULL);
	m_pConn.CreateInstance(__uuidof(Connection));
	m_pRs.CreateInstance(__uuidof(Recordset));
}

CAdoDB::~CAdoDB()
{
	//调用CoUninitialize释放COM对象
	::CoUninitialize();
}

BOOL CAdoDB::IsOpen()
{
	try
	{
		return ((m_pConn != NULL) && (m_pConn->State & adStateOpen));
	}
	catch(_com_error e)//异常处理
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
		MessageBox(NULL, _T("数据库未打开，执行SQL操作失败！"), _T("提示"), MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	try
	{//执行sql语句
		return m_pConn->Execute(_bstr_t(lpszSQL), &RecordsAffected, lOptions);
		//return m_pConn->Execute(_bstr_t(lpszSQL), NULL, lOptions);
	}
	catch(_com_error e)//异常处理
	{
	    MessageBox(NULL, e.ErrorMessage(), _T("数据库异常"),MB_OK|MB_ICONSTOP);
		MessageBox(NULL, e.Source(), _T("数据库异常"),MB_OK|MB_ICONSTOP);
		MessageBox(NULL, e.Description(), _T("数据库异常"),MB_OK|MB_ICONSTOP);
	}
	return FALSE;
}

BOOL CAdoDB::Open(LPCTSTR lpszConnect, long lOptions)
{
	//创建Connection对象
	//m_pConn.CreateInstance(_uuidof(Connection));
	//初始化Recordset指针
	//m_pRs.CreateInstance(_uuidof(Recordset));
	m_strConn = lpszConnect;
	if (m_strConn.IsEmpty())
	{
		return FALSE;
	}
	if (IsOpen()) Close();
	try
	{
		//数据库连接
		return (m_pConn->Open(_bstr_t(m_strConn), "", "", lOptions) == S_OK);
	}
	catch(_com_error e)//异常处理
	{
		AfxMessageBox(e.ErrorMessage());
		MessageBox(NULL,"打开数据库失败：\r请检查数据库是否在指定目录下!","错误",MB_OK);
	}
	return FALSE;
}

void  CAdoDB::Close()
{
	try
	{
		if ((m_pConn != NULL) && IsOpen())
		{
			//关闭数据库连接
			m_pConn->Close();
		}
	}
	catch(_com_error e)//异常处理
	{
		AfxMessageBox(e.ErrorMessage());
	}
}

void  CAdoDB::ExecuteQueryValue(LPCTSTR lpszSQL, CString& value)
{
	try
	{
		//打开记录集
		m_pRs->Open(_bstr_t(lpszSQL), 
		            (IDispatch*)m_pConn, 
					adOpenDynamic,
					adLockOptimistic,
					adCmdText);
	    if (!m_pRs->adoEOF)
		{
			//获取字段值
           _variant_t  thevalue = m_pRs->GetCollect(_variant_t((short)0));
		   value = vartostr(thevalue);
		}
		m_pRs->Close();
	}
	catch(_com_error e)//异常处理
	{
		AfxMessageBox(e.ErrorMessage());
	}
}

void  CAdoDB::ExecuteQuery(LPCTSTR lpszSQL, CDStrs& Fields)
{
	try
	{
		//打开记录集
		m_pRs->Open(_bstr_t(lpszSQL), 
		            (IDispatch*)m_pConn, 
					adOpenDynamic,
					adLockOptimistic,
					adCmdText);
		while(!m_pRs->adoEOF)
		{
			CStrs strs;
			_variant_t thevalue;
			//获取一条记录的所有字段值保存到字符串数组strs中
			int size = m_pRs->Fields->Count;
			for (int index = 0; index < size; index++){
				thevalue = m_pRs->GetCollect(_variant_t(index));
				CString temp = vartostr(thevalue);
				strs.push_back(temp);
			}
			//保存一条记录到二维数组中
			Fields.push_back(strs);
			//转到下一条记录
			m_pRs->MoveNext();
		}
		m_pRs->Close();
	}
	catch(_com_error e)//异常处理
	{
		AfxMessageBox(e.ErrorMessage());
	}
}

CString CAdoDB::vartostr(_variant_t& var)
{
	CString strValue;
	switch(var.vt)
	{
	//字符串
	case VT_BSTR:
	case VT_LPSTR:
	case VT_LPWSTR:
		strValue = (LPCTSTR) (_bstr_t)var;
		break;
	//BYTE类型
	case VT_I1:
	case VT_UI1:
		strValue.Format("%d", var.bVal);
		break;
	//短整型
	case VT_I2:
		strValue.Format("%d", var.iVal);
		break;
	//无符号短整型
	case VT_UI2:
		strValue.Format("%d", var.uiVal);
		break;
	//整型
	case VT_INT:
		strValue.Format("%d", var.intVal);
		break;
	//整型
	case VT_I4:
		strValue.Format("%d", var.lVal);
		break;
	//长整型
	case VT_I8:
		strValue.Format("%d", var.lVal);
		break;
	//无符号整型
	case VT_UINT:
		strValue.Format("%d", var.uintVal);
		break;
	//无符号整型
	case VT_UI4:
		strValue.Format("%d", var.ulVal);
		break;
	//无符号长整型
	case VT_UI8:
		strValue.Format("%d", var.ulVal);
		break;
	case VT_VOID:
		strValue.Format("%8x", var.byref);
		break;
	//浮点型，保持两位小数
	case VT_R4:
		strValue.Format("%.2f", var.fltVal);
		break;
	//双精度型，保持两位小数
	case VT_R8:
		strValue.Format("%.2f", var.dblVal);
		break;
	//数值型，保持两位小数
	case VT_DECIMAL:
		strValue.Format("%.2f", (double)var);
		break;
	//货币型
	case VT_CY:
		{
			COleCurrency cy = var.cyVal;
			strValue = cy.Format();
	    }
		break;
	//blob型数据
	case VT_BLOB:
	case VT_BLOB_OBJECT:
	case 0x2011:
		strValue = "[BLOB]";
		break;
	//布尔型
    case VT_BOOL:
		strValue = var.boolVal ? "TRUE" : "FALSE";
		break;
	//日期型
	case VT_DATE:
		{
			DATE dt = var.date;
			COleDateTime oleDt = COleDateTime(dt);
			strValue = oleDt.Format("%Y-%m-%d %H:%M:%S");
	    }
		break;
	//NULL型
	case VT_NULL:
		strValue = "NULL";
		break;
	case VT_EMPTY:
		strValue = "";
		break;
	//未知类型或者其他意外
	case VT_UNKNOWN:
	default:
	    strValue = "UN_KNOW";
		break;
	}
	return strValue;
}