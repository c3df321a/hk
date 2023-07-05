
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框




/*************************************************
函数名:    	CMFCApplication1Dlg
函数描述:	窗口内容初始化
输入参数:
输出参数:
返回值:
**************************************************/
CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_lPlayHandle = 0;
	CaptureRelease = true;
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_preset, m_comboPreset);
	DDX_Control(pDX, IDC_IPADDRESS_ip, m_ctrlDevIp);
	DDX_Control(pDX, IDC_EDIT_user, m_csUser);
	DDX_Control(pDX, IDC_EDIT_port, m_nDevPort);
	DDX_Control(pDX, IDC_EDIT_password, m_csPWD);
}


//点击按钮消息宏定义
BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//登入和IP框
	//ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_ip, &CMFCApplication1Dlg::OnIpnFieldchangedIpaddressip)
	ON_BN_CLICKED(IDC_BUTTON_login, &CMFCApplication1Dlg::OnBnClickedButtonlogin)
	//combobox，位置点按钮
	ON_BN_CLICKED(IDC_BUTTON_preset_delete, &CMFCApplication1Dlg::OnBnClickedButtonpresetdelete)
	ON_CBN_SELCHANGE(IDC_COMBO_preset, &CMFCApplication1Dlg::OnSelchangeComboPreset)
	ON_BN_CLICKED(IDC_BUTTON_preset_set, &CMFCApplication1Dlg::OnBnClickedButtonpresetset)
	ON_BN_CLICKED(IDC_BUTTON_preset_call, &CMFCApplication1Dlg::OnBnClickedButtonpresetcall)
	//云台控制
	ON_BN_CLICKED(IDC_BUTTON_up, &CMFCApplication1Dlg::OnBnClickedButtonup)
	ON_BN_CLICKED(IDC_BUTTON_down, &CMFCApplication1Dlg::OnBnClickedButtondown)
	ON_BN_CLICKED(IDC_BUTTON_left, &CMFCApplication1Dlg::OnBnClickedButtonleft)
	ON_BN_CLICKED(IDC_BUTTON_right, &CMFCApplication1Dlg::OnBnClickedButtonright)
	ON_BN_CLICKED(IDC_BUTTON_stop, &CMFCApplication1Dlg::OnBnClickedButtonstop)
	//播放/停止播放
	ON_BN_CLICKED(IDC_BUTTON_play, &CMFCApplication1Dlg::OnBnClickedButtonplay)
	//退出
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication1Dlg::OnBnClickedCancel)

	//二值化处理按钮
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::VedioBinary)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::VedioBinaryStop)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()//初始化dlg
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	//默认IP
	m_ctrlDevIp.SetAddress(192,168,1,64);//默认缺省值
	m_nDevPort.SetWindowText ( _T("8000"));
	m_csUser.SetWindowText (_T("admin"));
	m_csPWD.SetWindowText (_T("hk123456"));
	iPTZSpeed = 5;//默认速度

	m_bIsLogin = FALSE;//账户登入信息初始化

	m_bIsPlaying = false;//播放状态信息
	m_lPlayHandle = -1;//连接信息
	m_iCurChanIndex = 1;//默认是第一路视频播放；


	ShowWindow(SW_MINIMIZE);//小窗口

	
	//按钮可操作性控制（是否可以点击）
	
	//云台控制
	GetDlgItem(IDC_BUTTON_up)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_down)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_left)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_right)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_stop)->EnableWindow(FALSE);
	//播放按钮
	GetDlgItem(IDC_BUTTON_play)->EnableWindow(FALSE);
	//位置点设置
	GetDlgItem(IDC_BUTTON_preset_delete)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_preset)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_preset_set)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_preset_call)->EnableWindow(FALSE);

	//二值化处理
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


//mfc自动生成
void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


//mfc自动生成
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//mfc自动生成
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*************************************************
函数名:    	DoLogin
函数描述:	向设备注册
输入参数:
输出参数:
返回值:
**************************************************/
BOOL CMFCApplication1Dlg::DoLogin()
{
	

	//界面IP框初始化
	CString DeviceIp;
	//BYTE==undesigned char typedef unsigned char       BYTE;
	//获取IP
	BYTE nField0, nField1, nField2, nField3;
	//获取窗口信息
	m_ctrlDevIp.GetAddress(nField0, nField1, nField2, nField3);
	//赋值
	DeviceIp.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);

	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;

	//初始化
	memset(&DeviceInfoTmp, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	//获取消息框内容
	UpdateData(TRUE);

	//获取窗口内容
	CString str_port;
	m_nDevPort.GetWindowTextW(str_port);
	CString str_user;
	m_csUser.GetWindowTextW(str_user);
	CString str_password;
	m_csPWD.GetWindowTextW(str_password);
	//声明标识符
	USES_CONVERSION;

	//进行字符转换
	//参考自https://zhuanlan.zhihu.com/p/425996522
	char* CDeviceIp,*Cueser,*Cpassword;
	CDeviceIp = T2A(DeviceIp);
	Cueser = T2A(str_user);
	Cpassword = T2A(str_password);

	//因64位宽字节大小的改变因此不能在使用char*强转GetBuffer,这样会导致新数据仅有一个字符
	//LONG lLoginID = NET_DVR_Login_V30((char*)DeviceIp.GetBuffer(DeviceIp.GetLength()), _ttoi(str_port), \
		(char*)str_user.GetBuffer(str_user.GetLength()), (char*)str_password.GetBuffer(str_password.GetLength()), &DeviceInfoTmp);
	
	//用户设备注册
	LONG lLoginID = NET_DVR_Login_V30(CDeviceIp, _wtoi(str_port), Cueser, Cpassword, &DeviceInfoTmp);
	if (lLoginID == -1)
	{
		MessageBox(_T("设备登录失败"));
		return FALSE;
	}

	//云台控制
	GetDlgItem(IDC_BUTTON_up)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_down)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_left)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_right)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_stop)->EnableWindow(TRUE);
	//播放按钮
	GetDlgItem(IDC_BUTTON_play)->EnableWindow(TRUE);
	//位置点设置
	GetDlgItem(IDC_BUTTON_preset_delete)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_preset)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_preset_set)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_preset_call)->EnableWindow(TRUE);

	//初始化账户信息
	m_struDeviceInfo.lLoginID = lLoginID;//ID
	m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;//设备的通道数
	m_struDeviceInfo.iIPChanNum = DeviceInfoTmp.byIPChanNum;//最大数字通道个数
	m_struDeviceInfo.iStartChan = DeviceInfoTmp.byStartChan;//设备开始通道号
	m_struDeviceInfo.iIPStartChan = DeviceInfoTmp.byStartDChan;//数字通道起始通道号

	

	return TRUE;
}

/*************************************************
函数名:    	OnButtonLogin
函数描述:	注册/注销 按钮
输入参数:
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::OnBnClickedButtonlogin()
{
	if (!m_bIsLogin)    //login
	{
		if (!DoLogin())
			return;    
		GetDlgItem(IDC_BUTTON_login)->SetWindowText(_T("Logout"));
		GetDlgItem(IDOK)->EnableWindow(FALSE);//当摄像头登录成功后，不允许点击退出按钮
		GetDlgItem(IDC_STATIC_left)->EnableWindow(TRUE);//当摄像头登录成功后，允许点操作云台
		m_bIsLogin = TRUE;

	}
	else      //logout
	{ 
		if (m_bIsPlaying)
		{
			MessageBox(_T("Stop Play  first!"));
			return;
		}
		NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID);//Logout函数
		GetDlgItem(IDC_BUTTON_login)->SetWindowText(_T("Login"));//按钮文字变化
		GetDlgItem(IDOK)->EnableWindow(	TRUE);//当摄像头注销后，允许点击退出按钮
		GetDlgItem(IDC_STATIC_left)->EnableWindow(FALSE);//当摄像头注销后，不允许点操作云台
		m_struDeviceInfo.lLoginID = -1;//重初始化
		m_bIsLogin = FALSE;
	}
}


/*************************************************
函数名:    	InitDecoderReferCtrl
函数描述:	预制点按钮逻辑
输入参数:
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::InitDecoderReferCtrl()
{

	m_comboPreset.SetCurSel(0);
	
	GetDlgItem(IDC_BUTTON_preset_delete)->EnableWindow(FALSE);//让按钮无法点击
	GetDlgItem(IDC_BUTTON_preset_call)->EnableWindow(FALSE);
}


void CMFCApplication1Dlg::OnSelchangeComboPreset()
{
	// TODO: 在此添加控件通知处理程序代码
	int iIndex = m_comboPreset.GetCurSel();
	//判断位置点是否存在
	if (m_struDeviceInfo.struChanInfo[m_iCurChanIndex].struDecodercfg.bySetPreset[iIndex])
	{
		GetDlgItem(IDC_BUTTON_preset_delete)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_preset_call)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_preset_delete)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_preset_call)->EnableWindow(FALSE);
		
	}
}

// TODO: 在此添加控件通知处理程序代码

/*************************************************
函数名:    	OnButtonPresetSet
函数描述:	设置预置点
输入参数:
输出参数:
返回值:
**************************************************/

void CMFCApplication1Dlg::OnBnClickedButtonpresetset()
{
		int iPreset = m_comboPreset.GetCurSel() + 1;    //从零开始记+1
		if (m_lPlayHandle >= 0)
		{
			if (!NET_DVR_PTZPreset(m_lPlayHandle, SET_PRESET, iPreset))
			{
				MessageBox(_T("设置预置点失败"));
				return;
			}
		}

		//添加到预置点信息
		m_struDeviceInfo.struChanInfo[m_iCurChanIndex].struDecodercfg.bySetPreset[iPreset - 1] = TRUE;
		//更新按钮状态
		OnSelchangeComboPreset();

	}


/*************************************************
函数名:    	OnButtonPresetGoto
函数描述:	调用预置点
输入参数:
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::OnBnClickedButtonpresetcall()
{
	// TODO: 在此添加控件通知处理程序代码
	int iPreset = m_comboPreset.GetCurSel() + 1;    //+1
	if (m_lPlayHandle >= 0)
	{
		//播放视频时转动
		if (!NET_DVR_PTZPreset(m_lPlayHandle, GOTO_PRESET, iPreset))
		{
			MessageBox(_T("调用预置点失败"));
			return;
		}
	}
}

/*************************************************
函数名:    	OnBnClickedButtonpresetdelete
函数描述:	删除预制点
输入参数:
输出参数:
返回值:
**************************************************/

void CMFCApplication1Dlg::OnBnClickedButtonpresetdelete()
{
	// TODO: 在此添加控件通知处理程序代码
	//IDC_COMBO_preset

	int iPreset = m_comboPreset.GetCurSel() + 1;    //+1
	if (m_lPlayHandle >= 0)
	{
		if (!NET_DVR_PTZPreset(m_lPlayHandle, CLE_PRESET, iPreset))
		{
			MessageBox(_T("删除预置点失败"));
			return;
		}
	}
	else
	{
		if (!NET_DVR_PTZPreset_Other(m_struDeviceInfo.lLoginID, m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
			CLE_PRESET, iPreset))
		{
			MessageBox(_T("删除预置点失败"));
			return;
		}

	}

	//添加到预置点信息
	m_struDeviceInfo.struChanInfo[m_iCurChanIndex].struDecodercfg.bySetPreset[iPreset - 1] = FALSE;
	//更新按钮状态

	OnSelchangeComboPreset();

}


/*************************************************
函数名:    	OnBnClickedButtonup
函数描述:	云台上行控制
输入参数:
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::OnBnClickedButtonup()
{
	
	// TODO: 在此添加控件通知处理程序代码
	BOOL PTZControl = NET_DVR_PTZControlWithSpeed(m_lPlayHandle, TILT_UP, 0, iPTZSpeed);
		if (!PTZControl)
		{
			MessageBox(_T("云台控制失败!"));
			return;
		}
}

/*************************************************
函数名:    	OnBnClickedButtondown
函数描述:	云台下行控制
输入参数:
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::OnBnClickedButtondown()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL PTZControl = NET_DVR_PTZControlWithSpeed(m_lPlayHandle, TILT_DOWN, 0, iPTZSpeed);
	if (!PTZControl)
	{
		MessageBox(_T("云台控制失败!"));
		return;
	}
}

/*************************************************
函数名:    	OnBnClickedButtonleft
函数描述:	云台左行控制
输入参数:
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::OnBnClickedButtonleft()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL PTZControl = NET_DVR_PTZControlWithSpeed(m_lPlayHandle, PAN_LEFT, 0, iPTZSpeed);
	if (!PTZControl)
	{
		MessageBox(_T("云台控制失败!"));
		return;
	}
}

/*************************************************
函数名:    	OnBnClickedButtonright
函数描述:	云台右行控制
输入参数:
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::OnBnClickedButtonright()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL PTZControl = NET_DVR_PTZControlWithSpeed(m_lPlayHandle, PAN_RIGHT, 0, iPTZSpeed);
	if (!PTZControl)
	{
		MessageBox(_T("云台控制失败!"));
		return;
	}
}

/*************************************************
函数名:    	OnBnClickedButtonplay
函数描述:	点击播放视频
输入参数:
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::OnBnClickedButtonplay()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_iCurChanIndex == -1)
	{
		MessageBox(_T("确认具有正确的视频通道"));
		return;
	}
	if (!m_bIsPlaying)
	{
		StartPlay();//见下方定义
		m_bIsPlaying = TRUE;
		GetDlgItem(IDC_BUTTON_play)->SetWindowText(_T("停止播放"));
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		

	}
	else
	{
		if (CaptureRelease == false)
		{
			MessageBox(L"请先关闭二值化处理");
			return;
		}
		StopPlay();
		m_bIsPlaying = FALSE;
		GetDlgItem(IDC_BUTTON_play)->SetWindowText(_T("播放"));
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);

	}
}

/*************************************************
函数名:    	StartPlay
函数描述:	开始一路播放
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::StartPlay()
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd = GetDlgItem(IDC_STATIC_play)->m_hWnd;//在dlg中播放的位置
	ClientInfo.lChannel = m_iCurChanIndex ;//默认第一路播放，初始值放在初始化当中。
	ClientInfo.lLinkMode = 0;//解码流？不懂
	ClientInfo.sMultiCastIP = NULL;
	TRACE("Channel number:%d\n", ClientInfo.lChannel);
	//连接摄像头
	m_lPlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID, &ClientInfo, NULL, NULL, TRUE);
	if (-1 == m_lPlayHandle)
	{
		//如果连接失败的异常处理，并返回错误码
		DWORD err = NET_DVR_GetLastError();
		CString m_csErr;
		m_csErr.Format(_T("播放出错，错误代码%d"), err);
		MessageBox(m_csErr);
	}

	//摄像头正在播放
	m_bIsPlaying = TRUE;

	//更改按钮状态
	GetDlgItem(IDC_BUTTON_play)->SetWindowText(_T("停止播放"));

}

/*************************************************
函数名:    	StopPlay
函数描述:	停止播放
输入参数:
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::StopPlay()
{
	//如果不是关闭状态
	if (m_lPlayHandle != -1)
	{
		//关闭连接
		NET_DVR_StopRealPlay(m_lPlayHandle);
		//重初始化初始化数据
		//
		m_lPlayHandle = -1;
		//播放状态
		m_bIsPlaying = FALSE;
		GetDlgItem(IDC_STATIC_play)->Invalidate();
		GetDlgItem(IDC_STATIC_play)->ShowWindow(FALSE); //关闭窗口显示
		GetDlgItem(IDC_STATIC_play)->ShowWindow(TRUE); //打开窗口显示,相当于把窗体重启,清除Picture控件的残余
	}

}


/*************************************************
函数名:    	OnBnClickedCancel
函数描述:	程序退出按钮
输入参数:	
输出参数:
返回值:		
**************************************************/
void CMFCApplication1Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}


/*************************************************
函数名:    	PreTranslateMessage
函数描述:	esc，alt锁
输入参数:	MSG*
输出参数:	
返回值:		BOOL
**************************************************/


//递归调用消耗大量性能
BOOL CMFCApplication1Dlg::PreTranslateMessage(MSG* pMsg)

{

	if (pMsg->message == WM_KEYDOWN)

		if (pMsg->wParam == VK_ESCAPE)    //ESC  

			return TRUE;

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4

		return TRUE;



	return CDialog::PreTranslateMessage(pMsg);//递归调用

}




void CMFCApplication1Dlg::OnBnClickedButtonstop()
{
	// TODO: 在此添加控件通知处理程序代码
	bool	ret = NET_DVR_PTZControlWithSpeed(m_lPlayHandle, PAN_RIGHT, 1, iPTZSpeed);
	if (!ret)
	{
		MessageBox(_T("云台控制失败!"));
		return;
	}
}


/*************************************************
函数名:    	VedioBinary
函数描述:	图像二值化处理
输入参数:	
输出参数:
返回值:		
**************************************************/

void CMFCApplication1Dlg::VedioBinary()
{

	cv::namedWindow("ImageShow");//创建OpenCV窗口


	//嵌套opencv窗口
	HWND hWnd = (HWND)cvGetWindowHandle("ImageShow");
	HWND hParent = ::GetParent(hWnd);//获得父窗口
	::SetParent(hWnd, GetDlgItem(IDC_PICTURESHOW)->m_hWnd);//绑定窗口
	::ShowWindow(hParent, SW_HIDE);
	CWnd* pWnd = GetDlgItem(IDC_PICTURESHOW);//IDC_picture为picture控件ID
	pWnd->GetClientRect(&rc);//rc为控件的大小。


	//按钮逻辑控制
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);


	//抓取视屏内容，rtsp协议
	capture.open("rtsp://admin:hk123456@192.168.1.64");
	capture.set(cv::CAP_PROP_FPS, 30);
	CaptureRelease = false;
	//创建VideoCapture对象的另一种方式
	//VideoCapture capture("rtsp://admin:hikvision2021@192.168.1.64");

	/*int frameH = capture.get(4);
	int frameW = capture.get(3);
	cout << "frameH:" << frameH << "  frameW:" << frameW << endl;*/
	
	cv::Mat frame;
	cv::Mat imgGray;
	cv::Mat dst;

	//目前无法进行复杂的图像处理，图像处理速度太慢
	while (CaptureRelease==false)
	{
		capture.read(frame);
		cvtColor(frame, imgGray, cv::COLOR_BGR2GRAY);//灰度处理
		//cv::medianBlur(imgGray, imgGray, 5);//中值滤波
		threshold(imgGray, dst, 127, 255, cv::THRESH_BINARY);//二值化阈值处理
		//cv::adaptiveThreshold(imgGray, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);//自动阈值分割，高斯邻域
		//https ://blog.csdn.net/qq_43784519/article/details/127242922
		resize(dst, dst, cv::Size(rc.Width(), rc.Height()));
		imshow("ImageShow", dst);
		cv::waitKey(1);
		

	}

	//destroyWindow("test");
	//capture.release();//必须加release释放，否则会内存泄漏
}

/*************************************************
函数名:    	VedioBinaryStop
函数描述:	停止二值化处理
输入参数:
输出参数:
返回值:
**************************************************/
void CMFCApplication1Dlg::VedioBinaryStop()
{
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	// TODO: 在此添加控件通知处理程序代码
	cv::destroyWindow("ImageShow");
	capture.release();//必须加release释放，否则会内存泄漏
	CaptureRelease = true;
	GetDlgItem(IDC_PICTURESHOW)->ShowWindow(FALSE); //关闭窗口显示
	GetDlgItem(IDC_PICTURESHOW)->ShowWindow(TRUE); //打开窗口显示,相当于把窗体重启,清除Picture控件的残余
}

