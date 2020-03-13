
// jsMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "jsMFC.h"
#include "jsMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CjsMFCDlg �Ի���



CjsMFCDlg::CjsMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CjsMFCDlg::IDD, pParent)
	, mBigGuan(_T(""))
	, mSmallGuan(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CjsMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mBigGuan);
	DDV_MaxChars(pDX, mBigGuan, 1);
	DDX_Text(pDX, IDC_EDIT2, mSmallGuan);
	DDV_MaxChars(pDX, mSmallGuan, 2);
}

BEGIN_MESSAGE_MAP(CjsMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CjsMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CjsMFCDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CjsMFCDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CjsMFCDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CjsMFCDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CjsMFCDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CjsMFCDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON7, &CjsMFCDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CjsMFCDlg::OnBnClickedButton9)
END_MESSAGE_MAP()

int base = 0x007794f8;
HANDLE hProcess;

int *get2Point(int base, int p1, int p2)
{
	int iBase, iP1, *iP2;

	if (!ReadProcessMemory(hProcess, (LPVOID)base, &iBase, 4, NULL))
	{
		return NULL;
	}

	if (!ReadProcessMemory(hProcess, (LPVOID)(iBase + p1), &iP1, 4, NULL))
	{
		return NULL;
	}

	iP2 = (int *)(iP1 + p2);
	return iP2;
}

int *get3Point(int base, int p1, int p2, int p3)
{
	int iBase, iP1, iP2, *iP3;

	if (!ReadProcessMemory(hProcess, (LPVOID)base, &iBase, 4, NULL))
	{
		return NULL;
	}

	if (!ReadProcessMemory(hProcess, (LPVOID)(iBase + p1), &iP1, 4, NULL))
	{
		return NULL;
	}

	if (!ReadProcessMemory(hProcess, (LPVOID)(iP1 + p2), &iP2, 4, NULL))
	{
		return NULL;
	}
	iP3 = (int *)(iP2 + p3);
	return iP3;
}

BOOL InitWaigua()
{
	HWND hWnd = FindWindow(NULL, TEXT("Plants vs. Zombies 1.2.0.1073 �����ǲ���C++ѧԺ��"));
	if (NULL == hWnd)
	{
		//AfxMessageBox(TEXT("���Ҵ���ʧ��"));
		return FALSE;
	}

	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	//��ȡ���̾��
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		//AfxMessageBox(TEXT("�򿪽���ʧ��"));
		return FALSE;
	}

	return TRUE;
}

void ModifySun()
{
	int *sun = get2Point(base, 0x868, 0x5578);
	int sunValue = 9000;
	WriteProcessMemory(hProcess, sun, &sunValue, 4, NULL);
}

void ModifyMoney()
{
	int *pMoney = get2Point(base, 0x950, 0x50);
	int moneyValue = 500;
	WriteProcessMemory(hProcess, pMoney, &moneyValue, 4, NULL);
}

void ModifyColdTime()
{
	int *pCount = get3Point(base, 0x868, 0x15c, 0x24);
	if (pCount == NULL)
		return ;

	int *pFirst = get3Point(base, 0x868, 0x15c, 0x4c);
	if (pFirst == NULL)
		return ;

	//
	int iCount = 0;
	ReadProcessMemory(hProcess, pCount, &iCount, 4, NULL);
	for (int i = 0; i < iCount; i++)
	{
		//pFirst[0] = pFirst[1];//����д

		int iRecoveryTime;
		ReadProcessMemory(hProcess, pFirst + 1, &iRecoveryTime, 4, NULL);
		WriteProcessMemory(hProcess, pFirst, &iRecoveryTime, 4, NULL);

		pFirst = (int *)((int)pFirst + 0x50);
	}
}

// CjsMFCDlg ��Ϣ�������
BOOL IsSunUnlimite = FALSE, IsMoneyUnlimite = FALSE, IsColdTimeZero = FALSE;
DWORD WINAPI WaiguaThread(LPVOID lpParameter)
{
	CjsMFCDlg *dlg = (CjsMFCDlg *)lpParameter;
	while (1)
	{
		Sleep(1000);
		//�����Ϸ�Ƿ�����
		if (InitWaigua())
		{
			//��⵽����Ϸ
			dlg->SetDlgItemText((int)IDC_STATIC10, TEXT("�Ѿ���⵽��Ϸ����"));

			//����ԭ���������ֽ�
			//ReadProcessMemory(hProcess, pCode, oldOpCode, 3, NULL);
		}
		else
		{
			//���ô���״̬��û�м�⵽��Ϸ
			dlg->SetDlgItemText((int)IDC_STATIC10, TEXT("δ��⵽��Ϸ����"));
			continue;
		}


		if (IsSunUnlimite)
		{
			ModifySun();
		}
		if (IsMoneyUnlimite)
		{
			ModifyMoney();
		}
		if (IsColdTimeZero)
		{
			ModifyColdTime();
		}
	}


	return 0;
}

unsigned char *pCode = (unsigned char *)0x00472B50;
unsigned char oldOpCode[3];
unsigned char newOpCode[] = "\xC2\x04\x00";

BOOL CjsMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//�����߳�����ͣ�޸����⡣��������

	CreateThread(0, 0, WaiguaThread, this, 0, 0);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CjsMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CjsMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CjsMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CjsMFCDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	IsSunUnlimite = TRUE;
}


void CjsMFCDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	IsSunUnlimite = FALSE;
}


void CjsMFCDlg::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	IsMoneyUnlimite = TRUE;
}


void CjsMFCDlg::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	IsMoneyUnlimite = FALSE;
}


void CjsMFCDlg::OnBnClickedButton5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	IsColdTimeZero = TRUE;
}


void CjsMFCDlg::OnBnClickedButton6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	IsColdTimeZero = FALSE;
}

//������ͣ

void NoPause()
{
	DWORD dwOldProtect;
	VirtualProtectEx(hProcess, pCode, 3, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	WriteProcessMemory(hProcess, pCode, newOpCode, 3, NULL);
	VirtualProtectEx(hProcess, pCode, 3, dwOldProtect, NULL);
}

void YesPause()
{
	DWORD dwOldProtect;
	VirtualProtectEx(hProcess, pCode, 3, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	WriteProcessMemory(hProcess, pCode, oldOpCode, 3, NULL);
	VirtualProtectEx(hProcess, pCode, 3, dwOldProtect, NULL);
}

void CjsMFCDlg::OnBnClickedButton8()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	NoPause();
}


void CjsMFCDlg::OnBnClickedButton7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	YesPause();
}

void ModifyGuanka(int iGuanka)
{
	int *pGuanka = get2Point(base, 0x950, 0x4C);
	WriteProcessMemory(hProcess, pGuanka, &iGuanka, 4, NULL);
}

void CjsMFCDlg::OnBnClickedButton9()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData();

	TCHAR *pStopstring;
	long lBigGuan = wcstol(mBigGuan, &pStopstring, 10);
	long lSmallGuan = wcstol(mSmallGuan, &pStopstring, 10);
	if (lBigGuan < 1 || lBigGuan > 6)
	{
		AfxMessageBox(TEXT("��عؿ����ϸ�"));
		return;
	}
	if (lSmallGuan < 1 || lSmallGuan > 10)
	{
		AfxMessageBox(TEXT("С�عؿ����ϸ�"));
		return;
	}

	int iGuanka = (lBigGuan - 1) * 10 + lSmallGuan;
	ModifyGuanka(iGuanka);

	//AfxMessageBox(mBigGuan + mSmallGuan);
}
