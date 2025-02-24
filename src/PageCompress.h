#pragma once


// PageCompress 对话框

class PageCompress : public CDialogEx
{
	DECLARE_DYNAMIC(PageCompress)

public:
	PageCompress(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PageCompress();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStartcompress();
	CButton OffsetEncCheck;
	CButton AdcInfCheck;
	CEdit m_CSID;
	CEdit m_CSNm;
	CEdit m_CRID;
	CEdit m_CRNm;
	CEdit m_offset;
	afx_msg void OnBnClickedCheckOffset();
	afx_msg void OnBnClickedCheckAdcinf();
	CString m_CompressFilePath;
	BOOL OffsetEncFlag;
	BOOL AdcInfFlag;
	CString m_strCSID;
	CString m_strCSNm;
	CString m_strCRID;
	CString m_strCRNm;
	CString m_strOffset;
	BOOL xorEncFlag;
	BOOL rc4EncFlag;
	CString key;
	CEdit keyBox;
	CButton xorCheckBox;
	CButton rc4CheckBox;
	afx_msg void OnBnClickedCheckxorenc();
	afx_msg void OnBnClickedCheckrc4enccheck();
};
