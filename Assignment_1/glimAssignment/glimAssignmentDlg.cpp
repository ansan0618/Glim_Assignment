
// glimAssignmentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "glimAssignment.h"
#include "glimAssignmentDlg.h"
#include "afxdialogex.h"
#include <cmath>
#include <vector>
#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IMAGE_X 0
#define IMAGE_Y 25

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CglimAssignmentDlg 대화 상자



CglimAssignmentDlg::CglimAssignmentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIMASSIGNMENT_DIALOG, pParent)
	, nRadius(5)
	, nThick(3)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CglimAssignmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RADIUS, nRadius);
	DDX_Text(pDX, IDC_EDIT_THICK, nThick);
}

BEGIN_MESSAGE_MAP(CglimAssignmentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_SET_ZERO, &CglimAssignmentDlg::OnBnClickedBtnSetZero)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CglimAssignmentDlg::OnBnClickedBtnRandom)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CglimAssignmentDlg 메시지 처리기

BOOL CglimAssignmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	int nWidth = 950;
	int nHeight = 420;
	int nBpp = 8;

	m_image.Create(nWidth, nHeight, nBpp);
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0;i < 256;i++)
			rgb[i].rgbRed = rgb[i].rgbBlue = rgb[i].rgbGreen = i;
		m_image.SetColorTable(0, 256, rgb);

	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	for (int j = 0;j < nHeight;j++) {
		for (int i = 0; i < nWidth;i++) {
			fm[j * nPitch + i] = 0xff;
		}
	}
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CglimAssignmentDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CglimAssignmentDlg::OnPaint()
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		// 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_image.Draw(dc, IMAGE_X, IMAGE_Y);
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CglimAssignmentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void threadDrawCntCir(CglimAssignmentDlg* pDlg, int sttHeight, int endHeight, CPoint center, int nCntRadius) {
	int nWidth = pDlg->m_image.GetWidth();
	int nHeight = pDlg->m_image.GetHeight();
	int nPitch = pDlg->m_image.GetPitch();
	unsigned char* fm = (unsigned char*)pDlg->m_image.GetBits();
	CRect rect(0, 0, nWidth, nHeight);

	for (int j = sttHeight; j < endHeight;j++) {
			for (int i = center.x - nCntRadius; i < center.x + nCntRadius;i++) {
				int dX = i - center.x;
				int dY = j - center.y;
				int dist = dX * dX + dY * dY;
				dist = sqrt(dist);
				if (rect.PtInRect(CPoint(i, j))) {
					if (nCntRadius - (int)pDlg->nThick / 2 <= dist && dist <= nCntRadius + (int)pDlg->nThick / 2) {
							fm[j * nPitch + i] = 0x00;
					}
				}
			}
	}
}


void CglimAssignmentDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(true);
	mousePos = point;
	mousePos.x -= IMAGE_X;
	mousePos.y -= IMAGE_Y;

	int centerX = mousePos.x;
	int centerY = mousePos.y;

	BOOL moving = false;

	if (mousePos.x >= Circles[0].x - nRadius && mousePos.x <= Circles[0].x + nRadius &&
		mousePos.y >= Circles[0].y - nRadius && mousePos.y <= Circles[0].y + nRadius) {
		clickedCircle = 0;
		moving = true;
		m_clicked = true;
	}
	else if (mousePos.x >= Circles[1].x - nRadius && mousePos.x <= Circles[1].x + nRadius &&
		mousePos.y >= Circles[1].y - nRadius && mousePos.y <= Circles[1].y + nRadius) {
		clickedCircle = 1;
		moving = true;
		m_clicked = true;
	}
	else if (mousePos.x >= Circles[2].x - nRadius && mousePos.x <= Circles[2].x + nRadius &&
		mousePos.y >= Circles[2].y - nRadius && mousePos.y <= Circles[2].y + nRadius) {
		clickedCircle = 2;
		moving = true;
		m_clicked = true;
	}
	if (nCirNum < 3 && !moving) {
		clickToCircle(centerX, centerY);
		Circles[nCirNum++] = mousePos;
		moving = false;
	}
	showCenter(nCirNum);

	if (nCirNum == 3) {
		CPoint center;
		int nCntRadius;
		center = getCenter(Circles);
		nCntRadius = getCntRadius(center, Circles);

		int nHeight = m_image.GetHeight();
		int oneHeight = nHeight / 4;

		std::vector<std::thread> mThread;

		for (int i = 0;i < 4;i++) {
			int sttHeight = oneHeight * i;
			int endHeight = sttHeight + oneHeight;

			mThread.emplace_back(threadDrawCntCir, this, sttHeight, endHeight, center, nCntRadius);
		}

		for (auto& thread : mThread)
			thread.join();
		UpdateImg();
		nCirNum++;
	}
	UpdateData(false);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CglimAssignmentDlg::drawCircle(int i, int j, int centerX, int centerY, int nRadius) {
	int dX = (i - centerX);
	int dY = (j - centerY);
	double dist = dX * dX + dY * dY;
	
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	
	if (nRadius * nRadius > dist) {
		fm[j * nPitch + i] = 0x00;
	}
}

void CglimAssignmentDlg::showCenter(int nCirNum){
	CString sCenter;
	switch (nCirNum) {
	case 3:
		sCenter.Format(_T("(%d, %d)"), Circles[2].x, Circles[2].y);
		SetDlgItemText(IDC_STATIC_CENTER3, sCenter);
	case 2:
		sCenter.Format(_T("(%d, %d)"), Circles[1].x, Circles[1].y);
		SetDlgItemText(IDC_STATIC_CENTER2, sCenter);
	case 1:
		sCenter.Format(_T("(%d, %d)"), Circles[0].x, Circles[0].y);
		SetDlgItemText(IDC_STATIC_CENTER1, sCenter);
		break;
	}
}

CPoint CglimAssignmentDlg::getCenter(CPoint Circles[3]) {
	CPoint center;

	CPoint A = Circles[0];
	CPoint B = Circles[1];
	CPoint C = Circles[2];

	double D = 2 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
	
	center.x = (pow(A.x, 2) + pow(A.y, 2)) * (B.y - C.y) +
		(pow(B.x, 2) + pow(B.y, 2)) * (C.y - A.y) +
		(pow(C.x, 2) + pow(C.y, 2)) * (A.y - B.y);

	center.y = (pow(A.x, 2) + pow(A.y, 2)) * (C.x - B.x) +
		(pow(B.x, 2) + pow(B.y, 2)) * (A.x - C.x) +
		(pow(C.x, 2) + pow(C.y, 2)) * (B.x - A.x);

	center.x = center.x / D;
	center.y = center.y / D;

	return center;
}

int CglimAssignmentDlg::getCntRadius(CPoint center, CPoint circle[3]) {
	double nSum = 0;
	for (int i = 0;i < 3; i++) {
		auto dx = center.x - circle[i].x;
		auto dy = center.y - circle[i].y;
		nSum += dx * dx + dy * dy;
	}
	return sqrt(nSum / 3);
}

void CglimAssignmentDlg::clickToCircle(int centerX, int centerY) {
	for (int j = centerY - nRadius; j < centerY + (2 * nRadius);j++) {
		for (int i = centerX - nRadius; i < centerX + (2 * nRadius);i++) {
			drawCircle(i, j, centerX, centerY, nRadius);
		}
	}
	UpdateImg();
}


void CglimAssignmentDlg::OnBnClickedBtnSetZero()
{
	init();
}

void CglimAssignmentDlg::init(int a) {
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	for (int j = 0;j < nHeight;j++) {
		for (int i = 0; i < nWidth;i++) {
			fm[j * nPitch + i] = 0xff;
		}
	}

	UpdateImg();

	nCirNum = a;
	if (nCirNum == 0) {
		CString init = _T("(0, 0)");
		SetDlgItemText(IDC_STATIC_CENTER1, init);
		SetDlgItemText(IDC_STATIC_CENTER2, init);
		SetDlgItemText(IDC_STATIC_CENTER3, init);
	}
	else
		showCenter(nCirNum);
}

void CglimAssignmentDlg::OnBnClickedBtnRandom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	CString sRand;
	int nHeight = m_image.GetHeight() - 1;
	int nWidth = m_image.GetWidth() - 1;
	CPoint center;
	int nCntRadius;
	int x = rand() % nWidth;
	int y = rand() % nHeight;
	int oneHeight = nHeight / 4;
	
	for (int i = 0;i < 10;i++) {
		init();
		for (int j = 0;j < 3;j++) {
			if (nCirNum < 3) {
				x = rand() % nWidth;
				y = rand() % nHeight;
				
				clickToCircle(x, y);
				Circles[nCirNum++] = CPoint(x, y);
				showCenter(nCirNum);
			}
		}
		if (nCirNum >= 3) {
			center = getCenter(Circles);
			nCntRadius = getCntRadius(center, Circles);

			std::vector<std::thread> mThread;

			for (int i = 0;i < 4;i++) {
				int sttHeight = oneHeight * i;
				int endHeight = sttHeight + oneHeight;

				mThread.emplace_back(threadDrawCntCir, this, sttHeight, endHeight, center, nCntRadius);
			}

			for (auto& thread : mThread)
				thread.join();
			UpdateImg();
		}
		sRand.Format(_T("랜덤 이동 횟수 : %d"), i + 1);
		SetDlgItemText(IDC_STATIC_RANDOM, sRand);
		Sleep(2000);
	}
	SetDlgItemText(IDC_STATIC_RANDOM, _T("랜덤 이동 종료"));
	Sleep(1000);
	SetDlgItemText(IDC_STATIC_RANDOM, _T(" "));
	UpdateData(false);
}

void CglimAssignmentDlg::UpdateImg() {
	CClientDC dc(this);
	m_image.Draw(dc, IMAGE_X, IMAGE_Y);
}

void CglimAssignmentDlg::drawCntCircle(int i, int j, CPoint center, int nCntRadius, double dist) {
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	CRect rect(0, 0, nWidth, nHeight);
	
	if(rect.PtInRect(CPoint(i, j))){
		if (nCntRadius - nThick / 2 <= dist && dist <= nCntRadius + nThick / 2)
			fm[j * nPitch + i] = 0x00;
	}
	UpdateImg();
}

void CglimAssignmentDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_clicked) {
		m_clicked = false;
		showCenter(clickedCircle);
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CglimAssignmentDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_clicked) {
		Circles[clickedCircle] = CPoint(point.x - IMAGE_X, point.y - IMAGE_Y);
		if (nCirNum < 3) {
			init(nCirNum);
			showCenter(nCirNum);
			for (int k = 0;k < nCirNum;k++)
				clickToCircle(Circles[k].x, Circles[k].y);
		}
		else if (nCirNum >=3) {
			init(3);
			for (int k = 0;k < nCirNum;k++)
					clickToCircle(Circles[k].x, Circles[k].y);
			CPoint center;
			int nCntRadius;
			center = getCenter(Circles);
			nCntRadius = getCntRadius(center, Circles);
			
			int nHeight = m_image.GetHeight();
			int oneHeight = nHeight / 4;

			std::vector<std::thread> mThread;

			for (int i = 0;i < 4;i++) {
				int sttHeight = oneHeight * i;
				int endHeight = sttHeight + oneHeight;

				mThread.emplace_back(threadDrawCntCir, this, sttHeight, endHeight, center, nCntRadius);
			}

			for (auto& thread : mThread)
				thread.join();
			UpdateImg();
		}
		showCenter(nCirNum);
	}
	

	CDialogEx::OnMouseMove(nFlags, point);
}
