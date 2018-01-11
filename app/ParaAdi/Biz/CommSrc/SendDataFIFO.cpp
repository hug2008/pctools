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

    // 用于同步独立收发线程发送数据事件对象，手工重置，初试化为有信号的
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
    // 对前台发来的发送AT命令进行压入队列，保存消息体等处理
    SendFIFO *newrecord;
    BOOL     vaildcommand = TRUE;

    // 判断是否是当前有效的AT命令的类型，是否有不允许的重复情况存在
    //laurency
    vaildcommand = IsCurrentVaildCommandType(((SendData *)wParam)->commandtype);

    if (TRUE == vaildcommand)
    {
        // 是当前有效的AT命令的类型
        newrecord = (SendFIFO *)malloc(sizeof(SendFIFO));
        newrecord->senddata = ((SendData *)wParam)->senddata;
        newrecord->commandtype = ((SendData *)wParam)->commandtype;

        // 释放传递来的消息结构内存
        free((SendData *)wParam);

        m_pHeadFIFO = InsertRecord(m_pHeadFIFO, newrecord);

        // upgate maincontent work state
        if (m_pHeadFIFO != NULL)
        {

        }

        // 判断当前发送线程中是否有数据在发送，如果没有才发此新数据消息
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
        TRACE0("不是有效的AT命令\n");
        // 不是当前有效的AT命令的类型
        // 释放传递来的消息结构内存内包含的消息体内存
        free(((SendData *)wParam)->senddata);


        // 释放传递来的消息结构内存
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

            // 置当前发送线程中是否有数据的标识位为 FALSE，表示当前没有数据正在发送
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

            // 置当前发送线程中是否有数据的标识位为 FALSE，表示当前没有数据正在发送
            m_bIsSendingData = FALSE;
            break;

        default:
            AfxMessageBox("错误：OnSendFinishNotify消息处理函数接收到无效的消息内容！");
            break;
        }

        // 如果队列中有数据，则继续发送
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

            // 置当前发送线程中是否有数据的标识位为 TRUE，表示当前有数据正在发送
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

// 判断保存的命令队列中是否已经包含了相同的命令
BOOL CSendDataFIFO::IsExistSameAtCommand(CommandType commandtype)
{
    // 参数commandtype表示比较命令类型
    SendFIFO	*p0;
    BOOL		result = FALSE;

    // 得到保存要发送AT命令队列的头指针
    p0 = m_pHeadFIFO;
    if (p0 == NULL)
    {
        // 如果发送AT命令队列表为空，立即返回
    }
    else
    {
        while (p0 != NULL)
        {
            // 发送AT命令队列表不为空，就进行判断是否有相同的发送命令存在
            if (p0->commandtype == commandtype)
            {
                // 有相同的发送命令存在
                result = TRUE;
                break;
            }

            // 将数据链表指针指向下一个
            p0 = p0->next;
        }
    }

    // 返回查询的结果值
    return result;
}

// 判断是否是禁止重复的命令
BOOL CSendDataFIFO::IsForbidRepeatCommandType(CommandType commandtype)
{
    // 象下载全部短消息记录、下载全部电话本记录等操作是不允许同时重复执行
    BOOL	result = FALSE;

    if (commandtype == READ_TOTAL_SETTING_STATE_CMD||
            commandtype==WRITE_TOTAL_SETTING_STATE_CMD||
            commandtype==SAVE_TO_DEFAULT_SETTING_STATE_CMD||
            commandtype==RECOVER_DEFAULT_SETTING_STATE_CMD||
            commandtype==CHANGE_PASS_STATE_CMD)
    {
        // 不允许重复下载全部电话本记录
        result = TRUE;
    }
    return result;
}

// 判断是否是当前有效的AT命令的类型，是否有不允许的重复情况存在
BOOL CSendDataFIFO::IsCurrentVaildCommandType(CommandType commandtype)
{
    BOOL     result		  = FALSE;
    BOOL	 vaildcommand = TRUE;
    // 判断是否是禁止重复的命令
    result = IsForbidRepeatCommandType(commandtype);
    if (TRUE == result)
    {
        // 是禁止重复的命令，判断保存的命令队列中是否已经包含了相同的命令
        result = IsExistSameAtCommand(commandtype);
        if (TRUE == result)
        {
            // 已经包含了相同的命令
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