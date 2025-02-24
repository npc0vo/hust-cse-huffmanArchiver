
// HuffmanArchiverDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "HuffmanArchiver.h"
#include "HuffmanArchiverDlg.h"
#include "afxdialogex.h"
#include "util.h"
#define LINE_BREAK    _T("\r\n")
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


// CHuffmanArchiverDlg 对话框



CHuffmanArchiverDlg::CHuffmanArchiverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HUFFMANARCHIVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CHuffmanArchiverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrlMain);
	DDX_Control(pDX, IDC_EDIT1, m_EditLog);
}

BEGIN_MESSAGE_MAP(CHuffmanArchiverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CHuffmanArchiverDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CHuffmanArchiverDlg 消息处理程序

BOOL CHuffmanArchiverDlg::OnInitDialog()
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
	FILE* file = fopen(LogPath, "w");fclose(file);
	//初始化log文件
	
	//对TAB选项卡的子页进行位置调整
	ModifyStyle(WS_THICKFRAME | WS_MAXIMIZEBOX, 0);
	RECT tabRect;
	m_TabCtrlMain.GetClientRect(&tabRect);//获得选项卡区域大小的坐标(上下左右)
	tabRect.top += 26;//对坐标做一些微调，这26是选项卡按钮的高度
	tabRect.left += 2;//这是边框的宽度
	m_TabCtrlMain.InsertItem(0, _T("压缩"));
	m_TabCtrlMain.InsertItem(0, _T("解压缩"));
	m_PageCompress.Create(IDD_COMPRESS, &m_TabCtrlMain);
    m_PageDecompress.Create(IDD_DECOMPRESS, &m_TabCtrlMain);

	m_PageCompress.SetParent(&m_TabCtrlMain);
	m_PageDecompress.SetParent(&m_TabCtrlMain);

	m_PageCompress.MoveWindow(&tabRect);//把窗口移动到指定的区域里
	m_PageDecompress.MoveWindow(&tabRect);

	//默认设置显示第一个选项卡
	m_PageCompress.ShowWindow(SW_SHOW);
	m_PageDecompress.ShowWindow(SW_HIDE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHuffmanArchiverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHuffmanArchiverDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHuffmanArchiverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHuffmanArchiverDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	// 选项卡切换
	switch (m_TabCtrlMain.GetCurSel())
	{
		case 0:
			m_PageCompress.ShowWindow(SW_HIDE);
			m_PageDecompress.ShowWindow(SW_SHOW);
			break;
		case 1:
			m_PageCompress.ShowWindow(SW_SHOW);
			m_PageDecompress.ShowWindow(SW_HIDE);
			break;
		Default:
			break;
	}
	*pResult = 0;
}

/*
void CHuffmanArchiverDlg::Log(CString m_strLog)
{
	// 阻止CEdit控件重绘，防止界面闪烁
	m_EditLog.SetRedraw(FALSE);

	int nLineCount = m_EditLog.GetLineCount();

	// 如果输出日志超过1000行，则删除第一行
	if (nLineCount > 1000)
	{
		int nBegin = 0;
		int nEnd = nBegin + m_EditLog.LineLength(nBegin) + _tcslen(LINE_BREAK);

		m_EditLog.SetSel(nBegin, nEnd);
		m_EditLog.Clear();
	}

	// 每行输出日志结尾加上换行符
	m_strLog += LINE_BREAK;

	int nTextLen = m_EditLog.GetWindowTextLength();
	m_EditLog.SetSel(nTextLen, nTextLen, FALSE);
	m_EditLog.ReplaceSel(m_strLog);
	m_EditLog.LineScroll(m_EditLog.GetLineCount());

	m_EditLog.SetRedraw(TRUE);
}
*/

void CHuffmanArchiverDlg::Log(const TCHAR* format, ...)
{
	// 处理可变参数
	va_list args;
	va_start(args, format);

	// 用于存储格式化后的字符串
	TCHAR buffer[4096] = { 0 };
	_vstprintf_s(buffer, format, args);
	va_end(args);

	// 转换为CString
	CString m_strLog(buffer);

	// 阻止CEdit控件重绘，防止界面闪烁
	m_EditLog.SetRedraw(FALSE);

	int nLineCount = m_EditLog.GetLineCount();
	// 如果输出日志超过1000行，则删除第一行
	if (nLineCount > 1000)
	{
		int nBegin = 0;
		int nEnd = nBegin + m_EditLog.LineLength(nBegin) + _tcslen(LINE_BREAK);
		m_EditLog.SetSel(nBegin, nEnd);
		m_EditLog.Clear();
	}

	// 每行输出日志结尾加上换行符
	m_strLog += LINE_BREAK;
	int nTextLen = m_EditLog.GetWindowTextLength();
	m_EditLog.SetSel(nTextLen, nTextLen, FALSE);
	m_EditLog.ReplaceSel(m_strLog);
	m_EditLog.LineScroll(m_EditLog.GetLineCount());
	m_EditLog.SetRedraw(TRUE);
}