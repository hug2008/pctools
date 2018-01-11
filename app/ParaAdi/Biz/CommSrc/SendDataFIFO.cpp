// SendDataFIFO.cpp : implementation file
//

#include "stdafx.h"

#include "SendDataFIFO.h"
#include "MAINCONTENT.H"
#include "SERIALCOMM.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainContent MainContent;
extern CSerialComm MainSerial;

/////////////////////////////////////////////////////////////////////////////
// CSendDataFIFO

IMPLEMENT_DYNCREATE(CSendDataFIFO, CWinThread)

CSendDataFIFO::CSendDataFIFO()
{
    m_bIsSendingData = FALSE;
    m_pHeadFIFO      = NULL;

    // ����ͬ�������շ��̷߳��������¼������ֹ����ã����Ի�Ϊ���źŵ�
    if (MainContent.m_hPostSendingEvent)
        CloseHandle(MainContent.m_hPostSendingEvent);
    MainContent.m_hPostSendingEvent = CreateEvent(NULL, TRUE,
                                      TRUE, NULL);
}

CSendDataFIFO::~CSendDataFIFO()
{

}

BOOL CSendDataFIFO::InitInstance()
{
    // TODO:  perform and per-thread initialization here
    return TRUE;
}

int CSendDataFIFO::ExitInstance()
{
    // TODO:  perform any per-thread cleanup here
    ClearHeadFIFO();//add by wfx 2003.05.20
    if (MainContent.m_hPostSendingEvent)
        CloseHandle(MainContent.m_hPostSendingEvent);
    return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSendDataFIFO, CWinThread)
    //{{AFX_MSG_MAP(CSendDataFIFO)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
    ON_THREAD_MESSAGE(WM_SENDNEWFIFO_NOTIFY, OnSendNewFIFONotify)
    ON_THREAD_MESSAGE(WM_SENDFINISH_NOTIFY,  OnSendFinishNotify)
    ON_THREAD_MESSAGE(WM_CLEAR_SEND_DATA_NOTIFY,OnClearSendDataNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendDataFIFO message handlers

void CSendDataFIFO::OnSendNewFIFONotify(WPARAM wParam, LPARAM lParam)
{
    // ��ǰ̨�����ķ���AT�������ѹ����У�������Ϣ��ȴ���
    SendFIFO *newrecord;
    BOOL     vaildcommand = TRUE;

    // �ж��Ƿ��ǵ�ǰ��Ч��AT��������ͣ��Ƿ��в�������ظ��������
    //laurency
    vaildcommand = IsCurrentVaildCommandType(((SendData *)wParam)->commandtype);

    if (TRUE == vaildcommand)
    {
        // �ǵ�ǰ��Ч��AT���������
        newrecord = (SendFIFO *)malloc(sizeof(SendFIFO));
        newrecord->senddata = ((SendData *)wParam)->senddata;
        newrecord->commandtype = ((SendData *)wParam)->commandtype;

        // �ͷŴ���������Ϣ�ṹ�ڴ�
        free((SendData *)wParam);

        m_pHeadFIFO = InsertRecord(m_pHeadFIFO, newrecord);

        // upgate maincontent work state
        if (m_pHeadFIFO != NULL)
        {

        }

        // �жϵ�ǰ�����߳����Ƿ��������ڷ��ͣ����û�вŷ�����������Ϣ
        if (FALSE == m_bIsSendingData)
        {
            this->PostThreadMessage(
                WM_SENDFINISH_NOTIFY,
                FIFONEWDATA,
                0);
        }
    }
    else
    {
        TRACE0("������Ч��AT����\n");
        // ���ǵ�ǰ��Ч��AT���������
        // �ͷŴ���������Ϣ�ṹ�ڴ��ڰ�������Ϣ���ڴ�
        free(((SendData *)wParam)->senddata);


        // �ͷŴ���������Ϣ�ṹ�ڴ�
        free((SendData *)wParam);
    }


}
void CSendDataFIFO::OnClearSendDataNotify(WPARAM wParam,LPARAM lParam)
{
    ClearHeadFIFO();
}
void CSendDataFIFO::OnSendFinishNotify(WPARAM wParam, LPARAM lParam)
{
    if (WaitForSingleObject(MainContent.m_hPostSendingEvent, 0)
            == WAIT_OBJECT_0)
    {
        ResetEvent(MainContent.m_hPostSendingEvent);

        switch (wParam)
        {
        case FIFONEWDATA:

            break;

        case FIFOFINISH:

            // stop sending monitor timer
            MainSerial.m_hMainWnd->PostMessage(
                WM_STOP_MONITOR_TIMER_NOTIFY,
                0,
                0);

            if (m_pHeadFIFO)
            {

                m_pHeadFIFO = DeleteRecord(m_pHeadFIFO);

                // upgate maincontent work state
                if (m_pHeadFIFO == NULL)
                {

                }
            }

            // �õ�ǰ�����߳����Ƿ������ݵı�ʶλΪ FALSE����ʾ��ǰû���������ڷ���
            m_bIsSendingData = FALSE;
            break;

        case FIFOLOST:

            // stop sending monitor timer
            MainSerial.m_hMainWnd->PostMessage(
                WM_STOP_MONITOR_TIMER_NOTIFY,
                0,
                0);

            if (m_pHeadFIFO)
            {

                m_pHeadFIFO = DeleteRecord(m_pHeadFIFO);

                // upgate maincontent work state
                if (m_pHeadFIFO == NULL)
                {


                }
            }

            // �õ�ǰ�����߳����Ƿ������ݵı�ʶλΪ FALSE����ʾ��ǰû���������ڷ���
            m_bIsSendingData = FALSE;
            break;

        default:
            AfxMessageBox("����OnSendFinishNotify��Ϣ���������յ���Ч����Ϣ���ݣ�");
            break;
        }

        // ��������������ݣ����������
        if (m_pHeadFIFO)
        {

            ASSERT(MainContent.m_pCommThread);
            MainContent.m_pCommThread->PostThreadMessage(
                WM_SENDCOMM_NOTIFY,
                (unsigned int)m_pHeadFIFO,
                0);
            // start sending monitor timer
            MainSerial.m_hMainWnd->PostMessage(
                WM_START_MONITOR_TIMER_NOTIFY,
                0,
                0);

            // �õ�ǰ�����߳����Ƿ������ݵı�ʶλΪ TRUE����ʾ��ǰ���������ڷ���
            m_bIsSendingData = TRUE;
        }
        else
        {
            SetEvent(MainContent.m_hPostSendingEvent);
        }
    }


}

SendFIFO * CSendDataFIFO::InsertRecord(SendFIFO *head, SendFIFO *record)
{
    SendFIFO *p0, *p1;
    p1 = head;
    p0 = record;

    if (head == NULL)
    {
        head     = p0;
        p0->next = NULL;
    }
    else
    {
        while (p1->next != NULL)
        {
            p1 = p1->next;
        }

        p1->next = p0;
        p0->next = NULL;
    }

    return head;
}

SendFIFO * CSendDataFIFO::DeleteRecord(SendFIFO *head)
{
    SendFIFO *p0;

    if (head == NULL)
        return head;
    else
    {
        if (head->next == NULL)
        {
            if (head->senddata)
                free(head->senddata);

            free(head);
            head = NULL;
        }
        else
        {
            p0 = head->next;
            if (head->senddata)
                free(head->senddata);

            free(head);
            head = p0;
        }
    }

    return head;
}

// �жϱ��������������Ƿ��Ѿ���������ͬ������
BOOL CSendDataFIFO::IsExistSameAtCommand(CommandType commandtype)
{
    // ����commandtype��ʾ�Ƚ���������
    SendFIFO	*p0;
    BOOL		result = FALSE;

    // �õ�����Ҫ����AT������е�ͷָ��
    p0 = m_pHeadFIFO;
    if (p0 == NULL)
    {
        // �������AT������б�Ϊ�գ���������
    }
    else
    {
        while (p0 != NULL)
        {
            // ����AT������б�Ϊ�գ��ͽ����ж��Ƿ�����ͬ�ķ����������
            if (p0->commandtype == commandtype)
            {
                // ����ͬ�ķ����������
                result = TRUE;
                break;
            }

            // ����������ָ��ָ����һ��
            p0 = p0->next;
        }
    }

    // ���ز�ѯ�Ľ��ֵ
    return result;
}

// �ж��Ƿ��ǽ�ֹ�ظ�������
BOOL CSendDataFIFO::IsForbidRepeatCommandType(CommandType commandtype)
{
    // ������ȫ������Ϣ��¼������ȫ���绰����¼�Ȳ����ǲ�����ͬʱ�ظ�ִ��
    BOOL	result = FALSE;

    if (commandtype == READ_TOTAL_SETTING_STATE_CMD||
            commandtype==WRITE_TOTAL_SETTING_STATE_CMD||
            commandtype==SAVE_TO_DEFAULT_SETTING_STATE_CMD||
            commandtype==RECOVER_DEFAULT_SETTING_STATE_CMD||
            commandtype==CHANGE_PASS_STATE_CMD)
    {
        // �������ظ�����ȫ���绰����¼
        result = TRUE;
    }
    return result;
}

// �ж��Ƿ��ǵ�ǰ��Ч��AT��������ͣ��Ƿ��в�������ظ��������
BOOL CSendDataFIFO::IsCurrentVaildCommandType(CommandType commandtype)
{
    BOOL     result		  = FALSE;
    BOOL	 vaildcommand = TRUE;
    // �ж��Ƿ��ǽ�ֹ�ظ�������
    result = IsForbidRepeatCommandType(commandtype);
    if (TRUE == result)
    {
        // �ǽ�ֹ�ظ�������жϱ��������������Ƿ��Ѿ���������ͬ������
        result = IsExistSameAtCommand(commandtype);
        if (TRUE == result)
        {
            // �Ѿ���������ͬ������
            vaildcommand = FALSE;
        }
    }
    return vaildcommand;
}

//add by wfx 2003.05.20 begin
void CSendDataFIFO::ClearHeadFIFO()
{
    m_bIsSendingData = FALSE;
    while (m_pHeadFIFO!=NULL)
    {
        m_pHeadFIFO = DeleteRecord(m_pHeadFIFO);
    }
}
//add by wfx 2003.05.20 end