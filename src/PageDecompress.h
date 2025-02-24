#pragma once


// PageDecompress 对话框

class PageDecompress : public CDialogEx
{
	DECLARE_DYNAMIC(PageDecompress)

public:
	PageDecompress(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PageDecompress();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DECOMPRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	CString m_DeCompressFilePath;
	CString m_CodeFilePath;
	BOOL m_OffsetEncFlag;
	BOOL AdcInfFlag;
	CString m_strCSID;
	CString m_strCSNm;
	CString m_strCRID;
	CString m_strCRNm;
	afx_msg void OnBnClickedButtonDecompress();
	CString m_Offset;
	CEdit m_OffsetEdit;
	CEdit m_CSIDEdit;
	CEdit m_CSNmEdit;
	CEdit m_CRIDEdit;
	CEdit m_CRNmEdit;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheckAdcinf();
	CButton xorCheckBox;
	CButton rc4CheckBox;
	BOOL xorDecFlag;
	BOOL rc4DecFlag;
	CString key;
	afx_msg void OnBnClickedCheckxordec();
	afx_msg void OnBnClickedCheckrc4dec();
	CButton offsetCheckBox;
	BOOL decompressType;
	afx_msg void OnBnClickedCheckComparetype1();
	CButton treeCompareCheck;
	CButton oneCompareCheck;
	afx_msg void OnBnClickedCheckComparetype2();
	BOOL CompareType2;
};
