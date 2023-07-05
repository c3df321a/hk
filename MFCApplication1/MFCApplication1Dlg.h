#pragma once
// MFCApplication1Dlg.h: 头文件
//
#include "GeneralDef.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include<opencv2/highgui/highgui_c.h>

//这样使用名称空间有时会与系统的名称空间冲突
//using namespace cv;


// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	//声明消息相应函数
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonpresetdelete();
	BOOL DoLogin();//判断登入状态
	LOCAL_DEVICE_INFO m_struDeviceInfo;//存储本地设备信息
	int m_iCurChanIndex;    //默认摄像头在1通道           
	CComboBox m_comboPreset;//combo box
	LONG m_lPlayHandle;//接收NET_DVR_RealPlay_V30的返回值，并且作为是否播放成功的判断
	bool m_bIsPlaying;//摄像头播放检查
	bool m_bIsLogin;//账户登入信息检查
	int iPTZSpeed;//云台速度
	afx_msg void OnBnClickedButtonlogin();//登入
	void PresetControl();
	CIPAddressCtrl m_ctrlDevIp;//IP地址
	CEdit m_csUser;//用户名
	CEdit m_nDevPort;//端口
	CEdit m_csPWD;//摄像机密码

	//控件函数声明
	//afx_msg表明是全局函数
	afx_msg void OnSelchangeComboPreset();
	afx_msg void OnBnClickedButtonpresetset();
	afx_msg void OnBnClickedButtonpresetcall();
	afx_msg void OnBnClickedButtonup();
	afx_msg void OnBnClickedButtondown();
	afx_msg void OnBnClickedButtonleft();
	afx_msg void OnBnClickedButtonright();
	afx_msg void OnBnClickedButtonplay();

	//播放函数与停止函数
	void StartPlay();
	void StopPlay();

	//afx_msg void OnIpnFieldchangedIpaddressip(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnEnChangeEditport();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG*);//用于替代解决Alt f4，esc。
	afx_msg void OnBnClickedButtonstop();
	afx_msg void VedioBinary();

	afx_msg void VedioBinaryStop();
	cv::VideoCapture capture;
	BOOL CaptureRelease;
	CRect rc;//控件大小信息
};
