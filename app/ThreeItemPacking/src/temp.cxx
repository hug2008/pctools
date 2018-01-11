BOOL  CThreeItemPackingDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message){//键盘消息
		//收集键盘动作
		if (pMsg->message == VK_TAB){//不记录TAB键			
		}else if(pMsg->message == '\r'){
			MessageBox("捕获换行");
		}else if(pMsg->message == VK_SPACE){
			MessageBox("捕获空格");
		}
		else{
			m_strkeymsg = m_strkeymsg + (char)pMsg->wParam;
		}
		if  (pMsg->hwnd ==((GetDlgItem(IDC_EDIT1))->m_hWnd)){//这个消息属于TELNUM			
			if (VK_RETURN == pMsg->wParam){
				m_strTelnum = m_strkeymsg;
				m_strkeymsg.Format("Len = %d\nMsg = %s",m_strTelnum.GetLength(), m_strTelnum);
				MessageBox(m_strkeymsg);
				MySendkey->Send(Mykey);
				m_strkeymsg.Empty();
				return TRUE;
			}else{
				return CDialog::PreTranslateMessage(pMsg);
			}
		}else if (pMsg->hwnd ==((GetDlgItem(IDC_EDIT2))->m_hWnd)){//这个消息属于IMEI
			
			if (VK_RETURN == pMsg->wParam){
				m_strImei = m_strkeymsg;
				m_strkeymsg.Format("Len = %d\nMsg = %s",m_strImei.GetLength(), m_strImei);
				MessageBox(m_strkeymsg);
				MySendkey->Send(Mykey);
				m_strkeymsg.Empty();
				return TRUE;
			}else{
				return CDialog::PreTranslateMessage(pMsg);
			}
		}else if (pMsg->hwnd ==((GetDlgItem(IDC_EDIT3))->m_hWnd)){//这个消息属于CCID			
			if (VK_RETURN == pMsg->wParam){
				m_strCcid = m_strkeymsg;
				m_strkeymsg.Format("Len = %d\nMsg = %s",m_strCcid.GetLength(), m_strCcid);
				MessageBox(m_strkeymsg);
				EmptyCEidtItemInList();
				MySendkey->Send(Mykey);
				m_strkeymsg.Empty();
				return TRUE;
			}else{
				return CDialog::PreTranslateMessage(pMsg);
			}
		}else{
			//这个消息属于其他的窗口元素
			return CDialog::PreTranslateMessage(pMsg); 
		}
	}else{		
		return CDialog::PreTranslateMessage(pMsg); 
	}
}