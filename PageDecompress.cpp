// PageDecompress.cpp: 实现文件
//

#include "pch.h"
#include "HuffmanArchiver.h"
#include "PageDecompress.h"
#include "HuffmanArchiverDlg.h"
#include "afxdialogex.h"
#include "util.h"
#include "huffman.h"

// PageDecompress 对话框

IMPLEMENT_DYNAMIC(PageDecompress, CDialogEx)

PageDecompress::PageDecompress(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DECOMPRESS, pParent)
	, m_DeCompressFilePath(_T(""))
	, m_CodeFilePath(_T(""))
	, m_OffsetEncFlag(FALSE)
	, AdcInfFlag(FALSE)
	, m_strCSID(_T(""))
	, m_strCSNm(_T(""))
	, m_strCRID(_T(""))
	, m_strCRNm(_T(""))
	, m_Offset(_T("85"))
	, xorDecFlag(FALSE)
	, rc4DecFlag(FALSE)
	, key(_T(""))
	, decompressType(FALSE)
	, CompareType2(FALSE)
{

}

PageDecompress::~PageDecompress()
{
}
BOOL PageDecompress::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//将所有EditBox设置为不可编辑
	m_OffsetEdit.EnableWindow(FALSE);
	m_CSIDEdit.EnableWindow(FALSE);
	m_CSNmEdit.EnableWindow(FALSE);
	m_CRIDEdit.EnableWindow(FALSE);
	m_CRNmEdit.EnableWindow(FALSE);
	return TRUE;
}
void PageDecompress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_DeCompress, m_DeCompressFilePath);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_CodeFilePath, m_CodeFilePath);
	DDX_Check(pDX, IDC_CHECK1, m_OffsetEncFlag);
	DDX_Check(pDX, IDC_CHECK_AdcInf, AdcInfFlag);
	DDX_Text(pDX, IDC_EDIT_CSID, m_strCSID);
	DDX_Text(pDX, IDC_EDIT_CSNm, m_strCSNm);
	DDX_Text(pDX, IDC_EDIT_CRID, m_strCRID);
	DDX_Text(pDX, IDC_EDIT_CRNm, m_strCRNm);
	DDX_Text(pDX, IDC_EDIT_Offset, m_Offset);
	DDX_Control(pDX, IDC_EDIT_Offset, m_OffsetEdit);
	DDX_Control(pDX, IDC_EDIT_CSID, m_CSIDEdit);
	DDX_Control(pDX, IDC_EDIT_CSNm, m_CSNmEdit);
	DDX_Control(pDX, IDC_EDIT_CRID, m_CRIDEdit);
	DDX_Control(pDX, IDC_EDIT_CRNm, m_CRNmEdit);
	DDX_Control(pDX, IDC_CHECK_xorDec, xorCheckBox);
	DDX_Control(pDX, IDC_CHECK_rc4Dec, rc4CheckBox);
	DDX_Check(pDX, IDC_CHECK_xorDec, xorDecFlag);
	DDX_Check(pDX, IDC_CHECK_rc4Dec, rc4DecFlag);
	DDX_Text(pDX, IDC_EDIT_Key, key);
	DDX_Control(pDX, IDC_CHECK1, offsetCheckBox);
	DDX_Check(pDX, IDC_CHECK_CompareType1, decompressType);
	DDX_Control(pDX, IDC_CHECK_CompareType1, treeCompareCheck);
	DDX_Control(pDX, IDC_CHECK_CompareType2, oneCompareCheck);
	DDX_Check(pDX, IDC_CHECK_CompareType2, CompareType2);
}


BEGIN_MESSAGE_MAP(PageDecompress, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DeCompress, &PageDecompress::OnBnClickedButtonDecompress)
	ON_BN_CLICKED(IDC_CHECK1, &PageDecompress::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK_AdcInf, &PageDecompress::OnBnClickedCheckAdcinf)
	ON_BN_CLICKED(IDC_CHECK_xorDec, &PageDecompress::OnBnClickedCheckxordec)
	ON_BN_CLICKED(IDC_CHECK_rc4Dec, &PageDecompress::OnBnClickedCheckrc4dec)
	ON_BN_CLICKED(IDC_CHECK_CompareType1, &PageDecompress::OnBnClickedCheckComparetype1)
	ON_BN_CLICKED(IDC_CHECK_CompareType2, &PageDecompress::OnBnClickedCheckComparetype2)
END_MESSAGE_MAP()


// PageDecompress 消息处理程序




void PageDecompress::OnBnClickedButtonDecompress()
{
	// TODO: 在此添加控件通知处理程序代码
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

	// 格式化时间字符串
	CString strTime = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	pWnd->Log(TEXT("===============%s===================="), strTime);
	//获取文件路径
	//检测标志位，是否有偏移加密，是否有附加信息
	pWnd->Log(TEXT("[INFO]:文件路径:%s"), m_DeCompressFilePath);
	pWnd->Log(TEXT("[INFO]:编码表路径:%s"), m_CodeFilePath);
	int offset = _ttoi(m_Offset.GetString());
	if (m_OffsetEncFlag)
	{
		offset = _ttoi(m_Offset.GetString());

		//检查转换是否有效
		if (offset == 0 && m_Offset != _T("0")) {
			// 处理转换错误
			AfxMessageBox(TEXT("偏移值转换失败，请检查输入值是否符合！"), MB_OK | MB_ICONERROR);
			return;
		}
		pWnd->Log(TEXT("[INFO]:偏移解密已开启!"));
		pWnd->Log(TEXT("[INFO]:偏移值:%d"), offset);
	}
	else if (xorDecFlag)
	{
		pWnd->Log(TEXT("[INFO]:xor解密已开启!"));
		pWnd->Log(TEXT("[INFO]:key:%s"), key);
	}
	else if(rc4DecFlag)
	{ 
		pWnd->Log(TEXT("[INFO]:rc4解密已开启!"));
		pWnd->Log(TEXT("[INFO]:key:%s"), key);
	}
	if (AdcInfFlag)
	{
		pWnd->Log(TEXT("[INFO]:附加信息效验已开启!"));
		//pWnd->Log(TEXT("[INFO]:发送人学号:%s,发生人姓名:%s,接收人学号:%s,接收人姓名:%s"), m_strCSID, m_strCSNm, m_strCRID, m_strCRNm);
	}


	//开始压缩前将unicode字符串转为ASCII字符串
	int length = WideCharToMultiByte(CP_ACP, 0, m_DeCompressFilePath, -1, NULL, 0, NULL, NULL);
	char* inputFile = new char[length];
	WideCharToMultiByte(CP_ACP, 0, m_DeCompressFilePath, -1, inputFile, length, NULL, NULL);
	length = WideCharToMultiByte(CP_ACP, 0, m_CodeFilePath, -1, NULL, 0, NULL, NULL);
	char* codeFile = new char[length];
	WideCharToMultiByte(CP_ACP, 0, m_CodeFilePath, -1, codeFile, length, NULL, NULL);
	length = WideCharToMultiByte(CP_ACP, 0, m_strCSID, -1, NULL, 0, NULL, NULL);
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
	char* deckey= new char[length];
	WideCharToMultiByte(CP_ACP, 0, key, -1, deckey, length, NULL, NULL);


	if (m_DeCompressFilePath==TEXT(""))
	{
        AfxMessageBox(TEXT("请选择需要解压缩的文件！"), MB_OK | MB_ICONERROR);
		return;
	}
	if (m_CodeFilePath == TEXT(""))
	{
		AfxMessageBox(TEXT("请选择码表文件！"), MB_OK | MB_ICONERROR);
		return;
	}
	//进行解压缩
	mylog(LogPath, "[INFO]:输入文件路径:%s\n", inputFile);
	mylog(LogPath, "[INFO]:编码表文件路径:%s\n", codeFile);


	HuffmanArchiver Archiver(inputFile,codeFile);

	//计算耗时
	LARGE_INTEGER frequency;        // 计时器频率
	LARGE_INTEGER start;             // 开始时间
	LARGE_INTEGER end;               // 结束时间
	// 获取计时器的频率（每秒的计数次数）
	QueryPerformanceFrequency(&frequency);
	// 获取开始时间
	QueryPerformanceCounter(&start);
	bool status=Archiver.decompress(m_OffsetEncFlag, offset, AdcInfFlag, CSID, CSNm, CRID, CRNm,xorDecFlag,rc4DecFlag,deckey, decompressType);
	QueryPerformanceCounter(&end);
	CString adcInfo(Archiver.adcInfo);
	pWnd->Log(TEXT("[INFO]:附加信息:%s"), adcInfo);
	if (status)
	{
		double elapsedTime = (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
		double compressRate = ((double)Archiver.getInFileSize() / (double)Archiver.filesize) * 100;
		pWnd->Log(TEXT("[INFO]:解压缩完成!共耗时：%.5f ms,压缩码率为:%% %.2f"), elapsedTime,compressRate);
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
		memset(inFileHex, 0, sizeof(inFileHex));
		readLast16Bytes(Archiver.outFileName, inFileHex);
		pWnd->Log(TEXT("[INFO]:输出文件最后16字节:"));
		pWnd->Log(TEXT("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x"), inFileHex[0], inFileHex[1], inFileHex[2], inFileHex[3], inFileHex[4], inFileHex[5], inFileHex[6], inFileHex[7], inFileHex[8], inFileHex[9], inFileHex[10], inFileHex[11], inFileHex[12], inFileHex[13], inFileHex[14], inFileHex[15]);
		hash = fnv1a_64(Archiver.outFileName);
		pWnd->Log(TEXT("[INFO]:输出文件哈希值：0x%llx\n"), hash);
	}
	pWnd->Log(TEXT("================================================"));
}


void PageDecompress::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_OffsetEdit.EnableWindow(m_OffsetEncFlag);
	xorCheckBox.EnableWindow(!m_OffsetEncFlag);
	rc4CheckBox.EnableWindow(!m_OffsetEncFlag);
}


void PageDecompress::OnBnClickedCheckAdcinf()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_CSIDEdit.EnableWindow(AdcInfFlag);
	m_CSNmEdit.EnableWindow(AdcInfFlag);
	m_CRIDEdit.EnableWindow(AdcInfFlag);
	m_CRNmEdit.EnableWindow(AdcInfFlag);
}


void PageDecompress::OnBnClickedCheckxordec()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	offsetCheckBox.EnableWindow(!xorDecFlag);
	rc4CheckBox.EnableWindow(!xorDecFlag);
}


void PageDecompress::OnBnClickedCheckrc4dec()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	offsetCheckBox.EnableWindow(!rc4DecFlag);
    xorCheckBox.EnableWindow(!rc4DecFlag);
}


void PageDecompress::OnBnClickedCheckComparetype1()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	oneCompareCheck.EnableWindow(!decompressType);
}


void PageDecompress::OnBnClickedCheckComparetype2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	treeCompareCheck.EnableWindow(!CompareType2);

}
