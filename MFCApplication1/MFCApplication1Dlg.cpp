
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MY_CALENDAR, m_my_calendar);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(MCN_SELCHANGE, IDC_MY_CALENDAR, &CMFCApplication1Dlg::OnMcnSelchangeMyCalendar)
	ON_BN_CLICKED(IDC_SET_BTN, &CMFCApplication1Dlg::OnBnClickedSetBtn)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_font.CreatePointFont(160, L"굴림");
	m_my_calendar.SetFont(&m_font);
	
	OnMcnSelchangeMyCalendar(NULL, NULL);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

wchar_t* gp_week_str[7] = { L"일",L"월" ,L"화" ,L"수" ,L"목" ,L"금" ,L"토" }; //요일 리스트


void CMFCApplication1Dlg::OnMcnSelchangeMyCalendar(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	SYSTEMTIME cur_time; //타임 타입 형태
	m_my_calendar.GetCurSel(&cur_time);  // 캘린더에 타임형태 변수 대입
	
	CString str;
	str.Format(L"%d년 %d월 %d일[%s요일]", cur_time.wYear, cur_time.wMonth, cur_time.wDay, gp_week_str[cur_time.wDayOfWeek]); //선택한 날짜 텍스트박스에 보이도록 설정

	SetDlgItemText(IDC_DATE_STATIC, str); // 텍스트 박스 텍스트 설정

	str.Format(L"./%04d%02d%02d.dat", cur_time.wYear, cur_time.wMonth, cur_time.wDay); // 파일이름
	FILE* p_file;
	wchar_t temp_str[1024];
	if (0 == _wfopen_s(&p_file, str, L"rt,ccs=UNICODE")) { //파일 읽기
		str.Empty();//원래 내용 지움
		while (fgetws(temp_str, 1024, p_file) != NULL) {
			printf("%s", temp_str);
			str += temp_str;
		}
		
		fclose(p_file);

		str.Replace(L"\n", L"\r\n");// 텍스트 내용을 바꿈
		SetDlgItemText(IDC_NOTE_EDIT, str);
	}else {
		SetDlgItemText(IDC_NOTE_EDIT, L"");
	}

	if(pResult != NULL) *pResult = 0;
}


void CMFCApplication1Dlg::OnBnClickedSetBtn()
{	
	SYSTEMTIME cur_time;
	m_my_calendar.GetCurSel(&cur_time);
	CString str;
    str.Format(L"./%04d%02d%02d.dat", cur_time.wYear,cur_time.wMonth,cur_time.wDay); //파일 이름

	FILE* p_file;

	if (0 == _wfopen_s(&p_file, str, L"wt,ccs=UNICODE")) { //파일 쓰기
		GetDlgItemText(IDC_NOTE_EDIT, str);
		str.Replace(L"\r\n", L"\n");
		fwrite((const wchar_t *)str, (str.GetLength() + 1) * 2, 1, p_file);
		fclose(p_file);
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
