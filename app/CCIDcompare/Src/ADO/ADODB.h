#ifndef ADO_DB_H
#define ADO_DB_H
// ����ADO֧�ֿ�
#import "c:\program files\common files\system\ado\msado15.dll" \
	no_namespace \
	rename ("EOF", "adoEOF") 
//����vector����ṹ����һά�ַ�������
#include <vector>
typedef ::std::vector<CString> CStrs;
//����vector����ṹ�����ά�ַ�������
typedef ::std::vector<CStrs> CDStrs;

class CAdoDB
{
public:
	CAdoDB();
	virtual ~CAdoDB();
public:
	//���ݿ�����
	BOOL Open(LPCTSTR lpszConnect = _T(""), long lOptions = adConnectUnspecified);
	//���ݿ�ر�
	void Close();
	//�ж����ݿ��Ƿ��ڴ�״̬
	BOOL IsOpen();
	//�������ݿ�����ָ��
	_ConnectionPtr GetConnection() {return m_pConn;};
	//���ؼ�¼��ָ��
	_RecordsetPtr  GetRecordset()  {return m_pRs;}; 
	//���������ַ���
	CString GetConString()         {return m_strConn;};
	//ִ��SQL���
	_RecordsetPtr  Execute(LPCTSTR lpszSQL, long lOptions = adCmdText);
	//_variant_t���ݸ�ʽ������ת��Ϊ�ַ�����ʽ
	CString vartostr(_variant_t& var);
	//����SQL����ѯ���ݿ⣬��ȡ�ļ�¼�������ڶ�ά�ַ���������
	void ExecuteQuery(LPCTSTR lpszSQL, CDStrs& Fields);
	//����SQL����ѯ���ݿ⣬��ȡһ���ֶ�ֵ
	void ExecuteQueryValue(LPCTSTR lpszSQL, CString& value);
public:
	//�����ַ���
	CString           m_strConn;
	//���ݿ�����ָ��
	_ConnectionPtr    m_pConn;
	//��¼��ָ��
	_RecordsetPtr     m_pRs;
};
#endif