
// glimAssignmentDlg.h: 헤더 파일
//

#pragma once


// CglimAssignmentDlg 대화 상자
class CglimAssignmentDlg : public CDialogEx
{
// 생성입니다.
private:
	BOOL m_clicked = false;
	int clickedCircle;

public:
	CglimAssignmentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CPoint mousePos;
	CImage m_image;
	CPoint Circles[3];
	int nCirNum = 0;
	int nRadius;
	int nThick;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIMASSIGNMENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void drawCircle(int i, int j, int centerX, int centerY, int nRadius);
	void showCenter(int nCirNum);
	void clickToCircle(int centerX, int centerY);
	afx_msg void OnBnClickedBtnSetZero();
	afx_msg void OnBnClickedBtnRandom();
	void UpdateImg();
	CPoint getCenter(CPoint Circles[3]);
	void drawCntCircle(int i, int j, CPoint center, int nCntRadius, double dist);
	int getCntRadius(CPoint center, CPoint circle[3]);
	void init(int a = 0);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
