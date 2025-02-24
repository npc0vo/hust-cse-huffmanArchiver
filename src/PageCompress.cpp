// PageCompress.cpp: 实现文件
//

#include "pch.h"
#include "HuffmanArchiver.h"
#include "PageCompress.h"
#include "afxdialogex.h"
#include "HuffmanArchiverDlg.h"
#include "util.h"
#include "huffman.h"
// PageCompress 对话框

IMPLEMENT_DYNAMIC(PageCompress, CDialogEx)

PageCompress::PageCompress(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMPRESS, pParent)
	, m_CompressFilePath(_T(""))
	, m_strCSID(_T(""))
	, m_strCSNm(_T(""))
	, m_strCRID(_T(""))
	, m_strCRNm(_T(""))
	, m_strOffset(_T("85"))
	, xorEncFlag(FALSE)
	, rc4EncFlag(FALSE)
	, key(_T(""))
{

}

PageCompress::~PageCompress()
{
}

void PageCompress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Offset, m_offset);
	DDX_Control(pDX, IDC_CHECK_AdcInf, AdcInfCheck);
	DDX_Control(pDX, IDC_EDIT_CSID, m_CSID);
	DDX_Control(pDX, IDC_EDIT_CSNm, m_CSNm);
	DDX_Control(pDX, IDC_EDIT_CRID, m_CRID);
	DDX_Control(pDX, IDC_EDIT_CRNm, m_CRNm);
	DDX_Control(pDX, IDC_CHECK_Offset, OffsetEncCheck);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_CompressFilePath);
	DDX_Text(pDX, IDC_EDIT_CSID, m_strCSID);
	DDX_Text(pDX, IDC_EDIT_CSNm, m_strCSNm);
	DDX_Text(pDX, IDC_EDIT_CRID, m_strCRID);
	DDX_Text(pDX, IDC_EDIT_CRNm, m_strCRNm);
	DDX_Text(pDX, IDC_EDIT_Offset, m_strOffset);
	DDX_Check(pDX, IDC_CHECK_xorEnc, xorEncFlag);
	DDX_Check(pDX, IDC_CHECK2, rc4EncFlag);
	DDX_Text(pDX, IDC_EDIT_Key, key);
	DDX_Control(pDX, IDC_EDIT_Key, keyBox);
	DDX_Control(pDX, IDC_CHECK_xorEnc, xorCheckBox);
	DDX_Control(pDX, IDC_CHECK_rc4EncCheck, rc4CheckBox);
}


BEGIN_MESSAGE_MAP(PageCompress, CDialogEx)
	ON_BN_CLICKED(IDC_StartCompress, &PageCompress::OnBnClickedStartcompress)
	ON_BN_CLICKED(IDC_CHECK_Offset, &PageCompress::OnBnClickedCheckOffset)
	ON_BN_CLICKED(IDC_CHECK_AdcInf, &PageCompress::OnBnClickedCheckAdcinf)
	ON_BN_CLICKED(IDC_CHECK_xorEnc, &PageCompress::OnBnClickedCheckxorenc)
	ON_BN_CLICKED(IDC_CHECK_rc4EncCheck, &PageCompress::OnBnClickedCheckrc4enccheck)
END_MESSAGE_MAP()


BOOL PageCompress::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//将所有EditBox设置为不可编辑
	m_offset.EnableWindow(FALSE);
	m_CSID.EnableWindow(FALSE);
	m_CSNm.EnableWindow(FALSE);
	m_CRID.EnableWindow(FALSE);
	m_CRNm.EnableWindow(FALSE);
	return TRUE;
}
// PageCompress 消息处理程序


void PageCompress::OnBnClickedStartcompress()
{
	// TODO: 在此添加控件通知处理程序代码

	//同步UI变量
	UpdateData(TRUE);
	//获取主窗口指针
	CHuffmanArchiverDlg* pWnd = (CHuffmanArchiverDlg*)AfxGetMainWnd();

	
	if (pWnd == NULL)
	{
		// 找到了控件窗口
		int count = 0;
		pWnd->Log(TEXT("[WARNING]:初始化失败!"));
	}


	// 获取当前时间
	CTime currentTime = CTime::GetCurrentTime();

	if (m_CompressFilePath==TEXT(""))
	{
		AfxMessageBox(TEXT("请选择需要压缩的文件！"), MB_OK | MB_ICONERROR);
		return;
	}
	// 格式化时间字符串
	CString strTime = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	pWnd->Log(TEXT("===============%s===================="), strTime);
	pWnd->Log(TEXT("[INFO]:文件路径:%s"), m_CompressFilePath);

	//获取文件路径
	//检测标志位，是否有偏移加密，是否有附加信息
	int offset = _ttoi(m_strOffset.GetString());
	if (OffsetEncFlag)
	{
		offset = _ttoi(m_strOffset.GetString());

		//检查转换是否有效
		if (offset == 0 && m_strOffset != _T("0")) {
			// 处理转换错误
			AfxMessageBox(TEXT("偏移值转换失败，请检查输入值是否符合！"), MB_OK | MB_ICONERROR);
			return;
		}
		pWnd->Log(TEXT("[INFO]:偏移加密已开启!"));
		pWnd->Log(TEXT("[INFO]:偏移值:%d"), offset);
	}
	else if (rc4EncFlag)
	{
		pWnd->Log(TEXT("[INFO]:RC4加密已开启!"));
		pWnd->Log(TEXT("[INFO]:key:%s"), key);
	}
	else if (xorEncFlag)
	{
		pWnd->Log(TEXT("[INFO]:xor加密已开启!"));
		pWnd->Log(TEXT("[INFO]:key:%s"), key);
	}
	if (AdcInfFlag)
	{
		pWnd->Log(TEXT("[INFO]:附加信息已开启!"));
		pWnd->Log(TEXT("[INFO]:发送人学号:%s,发生人姓名:%s,接收人学号:%s,接收人姓名:%s"), m_strCSID,m_strCSNm,m_strCRID,m_strCRNm);
	}

	
	//开始压缩前将unicode字符串转为ASCII字符串
	int length = WideCharToMultiByte(CP_ACP, 0, m_CompressFilePath, -1, NULL, 0, NULL, NULL);
	char* inputFile = new char[length];
	WideCharToMultiByte(CP_ACP, 0, m_CompressFilePath, -1, inputFile, length, NULL, NULL);
	length= WideCharToMultiByte(CP_ACP, 0, m_strCSID, -1, NULL, 0, NULL, NULL);
	char* CSID = new char[length];
	WideCharToMultiByte(CP_ACP, 0, m_strCSID, -1, CSID, length, NULL, NULL);
	length = WideCharToMultiByte(CP_ACP, 0, m_strCSNm, -1, NULL, 0, NULL, NULL);
	char* CSNm = new char[length];
	WideCharToMultiByte(CP_ACP, 0, m_strCSNm, -1, CSNm, length, NULL, NULL);
	length = WideCharToMultiByte(CP_ACP, 0, m_strCRID, -1, NULL, 0, NULL, NULL);
	char* CRID = new char[length];
	WideCharToMultiByte(CP_ACP, 0, m_strCRID, -1, CRID, length, NULL, NULL);
	length = WideCharToMultiByte(CP_ACP, 0, m_strCRNm, -1, NULL, 0, NULL, NULL);
	char* CRNm = new char[length];
	WideCharToMultiByte(CP_ACP, 0, m_strCRNm, -1, CRNm, length, NULL, NULL);
	length = WideCharToMultiByte(CP_ACP, 0, key, -1, NULL, 0, NULL, NULL);
	char* enckey = new char[length];
	WideCharToMultiByte(CP_ACP, 0, key, -1, enckey, length, NULL, NULL);
	mylog(LogPath, "[INFO]:输入文件路径:%s\n", inputFile);

	//进行压缩
	HuffmanArchiver Archiver(inputFile);

	//计算耗时
	LARGE_INTEGER frequency;        // 计时器频率
	LARGE_INTEGER start;             // 开始时间
	LARGE_INTEGER end;               // 结束时间
	// 获取计时器的频率（每秒的计数次数）
	QueryPerformanceFrequency(&frequency);
	// 获取开始时间
	QueryPerformanceCounter(&start);
	Archiver.compress(OffsetEncFlag, offset, AdcInfFlag, CSID, CSNm, CRID, CRNm,xorEncFlag,rc4EncFlag,enckey);
	QueryPerformanceCounter(&end);
	double elapsedTime = (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
	double compressRate = ((double)Archiver.getOutFileSize() / (double)Archiver.filesize) * 100;
	pWnd->Log(TEXT("[INFO]:压缩完成!共耗时：%.5f ms,压缩码率为:%%%.2f"), elapsedTime,compressRate);
	unsigned char inFileHex[16] = { 0 };
	readLast16Bytes(Archiver.inFileName, inFileHex);
	pWnd->Log(TEXT("[INFO]:输入文件最后16字节:"));
	pWnd->Log(TEXT("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x"), inFileHex[0], inFileHex[1], inFileHex[2], inFileHex[3], inFileHex[4], inFileHex[5], inFileHex[6], inFileHex[7], inFileHex[8], inFileHex[9], inFileHex[10], inFileHex[11], inFileHex[12], inFileHex[13], inFileHex[14], inFileHex[15]);
	uint64_t hash = fnv1a_64(Archiver.inFileName);
	pWnd->Log(TEXT("[INFO]:输入文件哈希值：0x%llx\n"), hash);
	memset(inFileHex, 0, sizeof(inFileHex));
	readLast16Bytes(Archiver.codeFileName, inFileHex);
	pWnd->Log(TEXT("[INFO]:码表文件最后16字节:"));
	pWnd->Log(TEXT("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x"), inFileHex[0], inFileHex[1], inFileHex[2], inFileHex[3], inFileHex[4], inFileHex[5], inFileHex[6], inFileHex[7], inFileHex[8], inFileHex[9], inFileHex[10], inFileHex[11], inFileHex[12], inFileHex[13], inFileHex[14], inFileHex[15]);
	hash = fnv1a_64(Archiver.codeFileName);
    pWnd->Log(TEXT("[INFO]:码表文件哈希值：0x%llx\n"), hash);
	pWnd->Log(TEXT("[INFO]:WPL:%d"), Archiver.getWPL());
	memset(inFileHex, 0, sizeof(inFileHex));
	readLast16Bytes(Archiver.outFileName, inFileHex);
	pWnd->Log(TEXT("[INFO]:输出文件最后16字节:"));
	pWnd->Log(TEXT("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x"), inFileHex[0], inFileHex[1], inFileHex[2], inFileHex[3], inFileHex[4], inFileHex[5], inFileHex[6], inFileHex[7], inFileHex[8], inFileHex[9], inFileHex[10], inFileHex[11], inFileHex[12], inFileHex[13], inFileHex[14], inFileHex[15]);
	hash = fnv1a_64(Archiver.outFileName);
	pWnd->Log(TEXT("[INFO]:输出文件哈希值：0x%llx\n"), hash);
	pWnd->Log(TEXT("================================================"));
}



void PageCompress::OnBnClickedCheckOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	//控制偏移量的编辑窗口
	OffsetEncFlag = OffsetEncCheck.GetCheck();
	m_offset.EnableWindow(OffsetEncFlag);
	xorCheckBox.EnableWindow(!OffsetEncFlag);
    rc4CheckBox.EnableWindow(!OffsetEncFlag);
}


void PageCompress::OnBnClickedCheckAdcinf()
{
	// TODO: 在此添加控件通知处理程序代码
	AdcInfFlag = AdcInfCheck.GetCheck();
	m_CSID.EnableWindow(AdcInfFlag);
	m_CSNm.EnableWindow(AdcInfFlag);
    m_CRID.EnableWindow(AdcInfFlag);
	m_CRNm.EnableWindow(AdcInfFlag);
}





void PageCompress::OnBnClickedCheckxorenc()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OffsetEncCheck.EnableWindow(!xorEncFlag);
    rc4CheckBox.EnableWindow(!xorEncFlag);
}


void PageCompress::OnBnClickedCheckrc4enccheck()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OffsetEncCheck.EnableWindow(!rc4EncFlag);
	xorCheckBox.EnableWindow(!rc4EncFlag);
}
