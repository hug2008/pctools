BOOL  CThreeItemPackingDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message){//������Ϣ
		//�ռ����̶���
		if (pMsg->message == VK_TAB){//����¼TAB��			
		}else if(pMsg->message == '\r'){
			MessageBox("������");
		}else if(pMsg->message == VK_SPACE){
			MessageBox("����ո�");
		}
		else{
			m_strkeymsg = m_strkeymsg + (char)pMsg->wParam;
		}
		if  (pMsg->hwnd ==((GetDlgItem(IDC_EDIT1))->m_hWnd)){//�����Ϣ����TELNUM			
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
		}else if (pMsg->hwnd ==((GetDlgItem(IDC_EDIT2))->m_hWnd)){//�����Ϣ����IMEI
			
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
		}else if (pMsg->hwnd ==((GetDlgItem(IDC_EDIT3))->m_hWnd)){//�����Ϣ����CCID			
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
			//�����Ϣ���������Ĵ���Ԫ��
			return CDialog::PreTranslateMessage(pMsg); 
		}
	}else{		
		return CDialog::PreTranslateMessage(pMsg); 
	}
}