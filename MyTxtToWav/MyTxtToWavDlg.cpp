
// MyTxtToWavDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTxtToWav.h"
#include "MyTxtToWavDlg.h"
#include "afxdialogex.h"

#define DLG_FILTER "txt Files (*.txt)|*.txt||" 
#define DLG_EXT   "* " 

//-------------------Wav �ļ�ͷ------------------
#include "../include/wavehead.h"
//Ĭ����Ƶͷ������
struct wave_pcm_hdr default_pcmwavhdr = 
{
	{ 'R', 'I', 'F', 'F' },
	0,
	{'W', 'A', 'V', 'E'},
	{'f', 'm', 't', ' '},
	16,
	1,
	1,
	16000,
	32000,
	2,
	16,
	{'d', 'a', 't', 'a'},
	0  
};
//-------------------Wav �ļ�ͷend----------------


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyTxtToWavDlg �Ի���
CMyTxtToWavDlg::CMyTxtToWavDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyTxtToWavDlg::IDD, pParent)
	, m_str_WavPath(_T(""))
	, m_b_Check_ShowLog(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//-----------------------------------------------------
	str_MyAppId = "appid=509894ae";

	str_Config = "ssm=1,auf=audio/L16;rate=16000,vcn=xiaoyan";

	synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;

	str_Text  = "Ѷ����������ͨ���ޡ�";

	m_str_WavPath = L"D:\\";

	chrtmep = new char();

	m_d_1.m_chr_WavPath = new char();

	m_d_1.m_chr_WavPath_Save = new char();

	m_d_1.m_chr_TxtFileName = new char();

	m_d_1.m_chr_TxtFileNoHouzuiming = new char();

	m_d_2.m_chr_WavPath = new char();

	m_d_2.m_chr_WavPath_Save = new char();

	m_d_2.m_chr_TxtFileName = new char();

	m_d_2.m_chr_TxtFileNoHouzuiming = new char();

	//-----------------------------------------------------
}
//��������
CMyTxtToWavDlg::~CMyTxtToWavDlg()
{
	My_timer = NULL;
	chrtmep = NULL;
	m_d_1.m_chr_WavPath = NULL;
	m_d_1.m_chr_WavPath_Save = NULL;
	m_d_1.m_chr_TxtFileName = NULL;
	m_d_1.m_chr_TxtFileNoHouzuiming = NULL;
	m_d_2.m_chr_WavPath = NULL;
	m_d_2.m_chr_WavPath_Save = NULL;
	m_d_2.m_chr_TxtFileName = NULL;
	m_d_2.m_chr_TxtFileNoHouzuiming = NULL;

	delete My_timer;
	delete chrtmep;
	delete m_d_1.m_chr_WavPath;
	delete m_d_1.m_chr_WavPath_Save;
	delete m_d_1.m_chr_TxtFileName;
	delete m_d_1.m_chr_TxtFileNoHouzuiming;
	delete m_d_2.m_chr_WavPath;
	delete m_d_2.m_chr_WavPath_Save;
	delete m_d_2.m_chr_TxtFileName;
	delete m_d_2.m_chr_TxtFileNoHouzuiming;
}

void CMyTxtToWavDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TxtfilePath, m_edit_TxtfilePath);
	DDX_Control(pDX, IDC_EDIT_WavPath, m_edit_WavPath);
	DDX_Control(pDX, IDC_LISTCtrl_Log, m_listCtrl_Log);
	DDX_Control(pDX, IDC_BT_StartTTS, m_bt_StartTTS);
	DDX_Control(pDX, IDC_STATIC_ShowNote, m_text_ShowNote);
	DDX_Text(pDX, IDC_EDIT_WavPath, m_str_WavPath);
	DDX_Control(pDX, IDC_LISTCtrl_Log2, m_listCtrl_Log2);
	DDX_Control(pDX, IDC_CHECK_ShowLog, m_ctl_Chech_ShowLog);
	DDX_Check(pDX, IDC_CHECK_ShowLog, m_b_Check_ShowLog);
	DDX_Control(pDX, IDC_COMBO_Engine, m_ctl_Combo_Engine);
	DDX_Control(pDX, IDC_COMBO_Informant, m_ctl_Combo_Informant);
}

BEGIN_MESSAGE_MAP(CMyTxtToWavDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_Txt, &CMyTxtToWavDlg::OnBnClickedBtTxt)
	ON_BN_CLICKED(IDC_BT_WavPath, &CMyTxtToWavDlg::OnBnClickedBtWavpath)
	ON_BN_CLICKED(IDC_BT_OpenWavPath, &CMyTxtToWavDlg::OnBnClickedBtOpenwavpath)
	ON_BN_CLICKED(IDC_BT_StartTTS, &CMyTxtToWavDlg::OnBnClickedBtStarttts)
	ON_NOTIFY(LVN_INSERTITEM, IDC_LISTCtrl_Log, &CMyTxtToWavDlg::OnInsertitemListctrlLog)
	ON_BN_CLICKED(IDC_CHECK_ShowLog, &CMyTxtToWavDlg::OnBnClickedCheckShowlog)
	ON_NOTIFY(LVN_INSERTITEM, IDC_LISTCtrl_Log2, &CMyTxtToWavDlg::OnInsertitemListctrlLog2)
END_MESSAGE_MAP()

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyTxtToWavDlg::OnPaint()
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
HCURSOR CMyTxtToWavDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// CMyTxtToWavDlg ��Ϣ�������

BOOL CMyTxtToWavDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//------------------------------------------------------------------------------------------------------------
	//����dump�ļ�
	SetUnhandledExceptionFilter(CrashReportEx);
	HMODULE	hKernel32;

	// Try to get MiniDumpWriteDump() address.
	hDbgHelp = LoadLibrary(_T("DBGHELP.DLL"));
	MiniDumpWriteDump_ = (MINIDUMP_WRITE_DUMP)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	//	d("hDbgHelp=%X, MiniDumpWriteDump_=%X", hDbgHelp, MiniDumpWriteDump_);

	// Try to get Tool Help library functions.
	hKernel32 = GetModuleHandle(_T("KERNEL32"));
	CreateToolhelp32Snapshot_ = (CREATE_TOOL_HELP32_SNAPSHOT)GetProcAddress(hKernel32, "CreateToolhelp32Snapshot");
	Module32First_ = (MODULE32_FIRST)GetProcAddress(hKernel32, "Module32First");
	Module32Next_ = (MODULE32_NEST)GetProcAddress(hKernel32, "Module32Next");
	//------------------------------------------------------------------------------------------------------------
	My_DelLog();
	//------------------------------------------------------------------------------------------------------------
	ModifyStyle(WS_THICKFRAME, 0, SWP_FRAMECHANGED|SWP_DRAWFRAME);


	//�������ڴ�С�������� LOG 
	// 	CRect rect;
	// 	GetClientRect(&rect);
	// 	rect.left = 
	// 	rect.bottom = 160;
	// 	rect.right = 800;
	// 	AfxGetMainWnd()->MoveWindow(rect);
	// 	AfxGetMainWnd()->CenterWindow();

	m_listCtrl_Log.InsertColumn(0,L"�����Ϣ",0,620);
	m_listCtrl_Log2.InsertColumn(0,L"�����Ϣ",0,620);

	m_bt_StartTTS.EnableWindow(0);

	m_ctl_Combo_Informant.InsertString(0,L"xiaoyan������Ů����");
	m_ctl_Combo_Informant.InsertString(1,L"xiaoyu������������");
	m_ctl_Combo_Informant.InsertString(2,L"Catherine��Ӣ��Ů����");
	m_ctl_Combo_Informant.InsertString(3,L"henry��Ӣ��������	");
	m_ctl_Combo_Informant.InsertString(4,L"vixm��С÷�����");
	m_ctl_Combo_Informant.InsertString(5,L"vixl��С��̨����ͨ����");
	m_ctl_Combo_Informant.InsertString(6,L"vixr��С�أ��Ĵ�����");
	m_ctl_Combo_Informant.InsertString(7,L"vixyun��Сܿ����������");

	m_ctl_Combo_Informant.SetCurSel(0);

	InitializeFlatSB(m_hWnd);
	FlatSB_EnableScrollBar(m_hWnd, SB_BOTH, ESB_DISABLE_BOTH);

	//	TTS_Start();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CMyTxtToWavDlg::My_DelLog()
{
	TCHAR buffer[ MAX_PATH ];
	CString ExePath = _T("");
	GetModuleFileName( NULL, buffer, MAX_PATH );
	ExePath = CString( buffer );
	ExePath=ExePath.Left(
		ExePath.ReverseFind( '\\' )>ExePath.ReverseFind( '/' )?
		ExePath.ReverseFind( '\\' )+ 1:
	ExePath.ReverseFind( '/' )+1        
		);
	ExePath += L"msc.log";
	BOOL bjjk = DeleteFile(ExePath);
}
/*
���ܣ�
�������CString �����и������ù̶�
����ֵ��
����һ�����顣m_StrOut  ������
*/
m_StrOut CMyTxtToWavDlg::My_FenGe(CString poip, int istart, int ilength)
{
	m_StrOut m_strout;
	CString ioutstr , inowTemp;
	int iFind = 0;
	int iFind2 = 0;
	wchar_t * wchrDouhao = L"��";
	wchar_t * wchrJuhao = L"��";

	//�Ƚ���һ��Ĭ��ֵ���и�п��ܽ�β����'��'
	inowTemp = poip.Mid( istart, ilength);

	//Ȼ����з����'��'�Ĳ���
	iFind  = inowTemp.ReverseFind(*wchrJuhao);
	iFind2 = inowTemp.ReverseFind(*wchrDouhao);

	//���� �� λ�õĶԱ�
	//�����һλǡ���Ǿ�ŵ�ʱ��
	if ( iFind ==  ilength - 1)
	{
		m_strout.iNextStart = istart + ilength;
		m_strout.strOut = inowTemp;
		m_strout.iStartPot = istart;
		m_strout.iLength = inowTemp.GetLength();
	}
	//�����һλǡ���Ƕ��ŵ�ʱ��
	else if (iFind2 == ilength - 1)
	{
		m_strout.iNextStart = istart + ilength;
		m_strout.strOut = inowTemp;
		m_strout.iStartPot = istart;
		m_strout.iLength = inowTemp.GetLength();
	}
	//��� ��� �� ���� ���棬��ô�Ծ��Ϊ�ض�ĩβ
	else if (iFind > iFind2)
	{
		m_strout.iNextStart = istart + iFind + 1;
		m_strout.strOut = poip.Mid( istart, iFind + 1);
		m_strout.iStartPot = istart;
		m_strout.iLength = m_strout.strOut.GetLength();
	}
	//��� ��� �� ���� ���棬��ô�Ծ��Ϊ�ض�ĩβ
	else if (iFind < iFind2)
	{
		m_strout.iNextStart = istart + iFind2 + 1;
		m_strout.strOut = poip.Mid( istart, iFind2 + 1);
		m_strout.iStartPot = istart;
		m_strout.iLength = m_strout.strOut.GetLength();
	}
	else if (iFind == -1 && iFind2 == -1)
	{
		m_strout.iNextStart = istart + ilength;
		m_strout.strOut = inowTemp;
		m_strout.iStartPot = istart;
		m_strout.iLength = inowTemp.GetLength();
	}

	return m_strout;
}

UINT CMyTxtToWavDlg::My_thread_TTS_2(LPVOID lpParam)
{
	My_QTTSInit			* my_QTTSInit;
	My_QTTSSessionBegin * my_QTTSSessionBegin;
	My_QTTSTextPut		* my_QTTSTextPut;
	My_QTTSAudioGet		* my_QTTSAudioGet;
	My_QTTSGetParam		* my_QTTSGetParam;
	My_QTTSSessionEnd	* my_QTTSSessionEnd;
	My_QTTSFini			* my_QTTSFini;
	My_QTTSAudioInfo	* my_QTTSAudioInfo;

	HINSTANCE hDLLDrv = LoadLibrary(L".\\thread2\\msc.dll");

	//��ȡmessage������ָ��
	if(hDLLDrv)
	{
		my_QTTSInit			= (My_QTTSInit *)GetProcAddress(hDLLDrv, "QTTSInit");
		my_QTTSSessionBegin = (My_QTTSSessionBegin *)GetProcAddress(hDLLDrv, "QTTSSessionBegin");
		my_QTTSTextPut	    = (My_QTTSTextPut *)GetProcAddress(hDLLDrv, "QTTSTextPut");
		my_QTTSAudioGet     = (My_QTTSAudioGet *)GetProcAddress(hDLLDrv, "QTTSAudioGet");
		my_QTTSGetParam     = (My_QTTSGetParam *)GetProcAddress(hDLLDrv, "QTTSGetParam");
		my_QTTSSessionEnd   = (My_QTTSSessionEnd *)GetProcAddress(hDLLDrv, "QTTSSessionEnd");
		my_QTTSFini			= (My_QTTSFini *)GetProcAddress(hDLLDrv, "QTTSFini");
		my_QTTSAudioInfo	= (My_QTTSAudioInfo *)GetProcAddress(hDLLDrv, "QTTSAudioInfo");
	}


	LPWSTR	Lpw_temp =  new TCHAR[1024];
	TCHAR tchartemp[1024];

	//���洴��һ��WAV�ļ�������Ƶ��д��
	char* audio_data;
	unsigned int audio_len = 0;


	m_data * m_d = (m_data *)lpParam;

	const char*		str_MyAppId		= m_d->str_MyAppId;
	char*			str_Config		= m_d->str_Config;
	int				ret				;//= m_d->ret;
	const char*		sess_id			;//= m_d->sess_id;	
	int				synth_status	;//= m_d->synth_status;
	//���� �ָ�õ�txt����
	char*			str_Text		= m_d->m_chr_Txt;

	CMyTxtToWavDlg * m_MyTxtToWav	= m_d->m_MyTxtToWav;

	CListCtrl	   * m_listCtrl_Log ;

	if (m_d->iWhichThread == 1)
	{
		m_listCtrl_Log = m_d->mylistctrl;
	}
	else
	{
		m_listCtrl_Log = m_d->mylistctrl_2;
	}


	CButton		   * m_bt_StartTTS	= m_d->m_bt_StartTTS;

	const char*  tmpname = m_d->m_chr_WavPath;//"D:\\tts_audio_test.wav";

	CStatic		   * m_text_ShowNote = m_d->m_text_ShowNote;
	//	m_text_ShowNote->SetWindowTextW(L"�����ϳ�ing��������ص����������������ͨ����");

	m_listCtrl_Log->DeleteAllItems();
	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"��ʼ����TTS");


	//-----------------��ʼ��---------------------

	ret = my_QTTSInit( m_d->str_MyAppId );
	//	ret = QTTSInit( m_d->str_MyAppId);
	if ( ret != MSP_SUCCESS )
	{

		printf("QTTSInit: qtts init failed, ret = %d.\n", ret);

		wsprintf(Lpw_temp,L"QTTSInit: qtts init failed, ret = %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");

		return ret;
	}

	//���Σ� �Ựģʽ����Ƶ������������ѡ��
	sess_id = my_QTTSSessionBegin(m_d->str_Config, &ret);
	if ( ret != MSP_SUCCESS )
	{
		printf("QTTSSessionBegin: qtts begin session failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"QTTSSessionBegin: qtts begin session failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		if (ret > 0 && ret < 10100)
		{
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�������쳣�����Ժ����ԣ�");
		}

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");

		my_QTTSFini();
		return ret;
	}
	//-----------------��ʼ��end-------------------

	//������Ҫ�ϳɵ��ַ���
	ret = my_QTTSTextPut(sess_id, str_Text,strlen(str_Text) , NULL );//  sizeof(str_Text)

	if ( ret != MSP_SUCCESS )
	{
		printf("QTTSTextPut: qtts put text failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"QTTSTextPut: qtts put text failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");
		my_QTTSSessionEnd(sess_id, "");
		my_QTTSFini();
		return ret;
	}

	//���洴��һ��WAV�ļ�������Ƶ��д��
	FILE* fp = fopen(tmpname, "wb");
	if ( fp == NULL )
	{
		printf(" failed to open %s file.\n",tmpname);
		::MessageBox(NULL,L"����WAV��Ƶ�ļ�����!",L"error",0);


		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");

		my_QTTSSessionEnd(sess_id, "");
		my_QTTSFini();
		return ret;
	}

	fwrite(&default_pcmwavhdr, 1, sizeof(default_pcmwavhdr), fp);

	//------------------------------------------------------------

	int pos = 0;			//���ڱ����һ���Ѿ��ϳɵ���λ��
	int loop_count = 0;		//���ڱ�ǣ�ȡ�˼��ν��
	int upload_flow = 0,download_flow = 0;//�ϴ���������������
	char param_value[32] = "";//����ֵ���ַ�����ʽ
	size_t value_len = 32;	//�ַ������Ȼ�buffer����


	while ( true )
	{
		audio_data = (char*)my_QTTSAudioGet( sess_id ,&audio_len , &synth_status , &ret );
		/****************��ȡ�ϳ�λ��*************s****///�������Ҫ�ù��ܣ����ο���ɾ��
		const char* audio_info = my_QTTSAudioInfo(sess_id);	//��ȡ�Ѿ��õ�����Ƶ��λ��

		if (NULL == audio_info)
		{
			wsprintf(Lpw_temp,L"QTTSAudioInfo ��ȡ��Ƶλ����Ϣʧ��.");
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			continue;
		}


		int ced = atoi(audio_info+4);						//λ����Ϣ�ĸ�ʽ��ced=xxx������ǰ�ĸ��ַ����ض�����
		char text_complete[iAllLength];							//�����ַ��������Ѿ��ϳ���ϵ��ı���
		strcpy(text_complete,str_Text+pos);					//��ԭ�ַ������Ƶ����ַ����У�������ǰ�����нضϣ���֮ǰ�ϳɵģ��Ǳ��κϳɵ��ı�����
		text_complete[ced-pos]='\0';						//�����ַ������к󲿽ضϣ��ضϵ�ced��ʾ��λ�ã���������ַ������Ǳ��λ�ȡ�����ַ�����

		printf("[%d]:get result[err==%d / status==%d]:%s\n", (loop_count), ret, synth_status,text_complete);
		MultiByteToWideChar(CP_ACP,0,text_complete,-1,tchartemp,1024);
		wsprintf(Lpw_temp,L"[%d]:get result[err==%d / status==%d]:%s", (loop_count), ret, synth_status,tchartemp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		loop_count++;										//ȡ�����������
		pos = ced;											//�ϳ�λ�ø���
		/**************��ȡ�ϳ�λ�ý���***************/
		if ( ret != MSP_SUCCESS )
		{
			if (ret == MSP_ERROR_NO_MORE_DATA || ret ==  MSP_ERROR_NO_DATA || ret ==  MSP_ERROR_TTS_GENERAL)
			{
				//˵���Ѿ��������
				break;
			}
			printf("QTTSAudioGet: qtts get audio failed Error code %d.\n",ret);
			wsprintf(Lpw_temp,L"QTTSAudioGet: qtts get audio failed Error code %d.",ret);
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			//	char key = _getch();
			break;
		}
		/*********��ȡ��ǰ�Ự��ʱ��������Ϣ**********///�������Ҫ�ù��ܣ����ο���ɾ��
		value_len = 32;//value_len���Ǵ�����������Ǵ���������ÿ�ε���QTTSGetParamʱҪ����Ϊbuffer����
		ret = my_QTTSGetParam(sess_id,"upflow",param_value,&value_len);//��ȡ����������Ϣ
		if ( ret != MSP_SUCCESS )
		{
			printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
			wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.",ret);
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			break;
		}
		upload_flow = atoi(param_value);

		printf("upflow== %d Byte\n",upload_flow);
		wsprintf(Lpw_temp,L"upflow== %d Byte.",upload_flow);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		value_len = 32;//value_len���Ǵ�����������Ǵ���������ÿ�ε���QTTSGetParamʱҪ����Ϊbuffer����
		ret = my_QTTSGetParam(sess_id,"downflow",param_value,&value_len);//��ȡ����������Ϣ
		if ( ret != MSP_SUCCESS )
		{
			printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
			wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.\n",ret);
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			break;
		}
		download_flow = atoi(param_value);

		printf("downflow== %d Byte\n",download_flow);
		wsprintf(Lpw_temp,L"downflow== %d Byte",download_flow);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		/**************��ȡ������Ϣ����***************/
		fwrite(audio_data, 1, audio_len, fp);
		//	fwrite(audio_data, sizeof(audio_data), 1, fp);
		default_pcmwavhdr.data_size += audio_len;//����pcm���ݵĴ�С
		if ( MSP_TTS_FLAG_DATA_END == synth_status )
		{
			printf("QTTSAudioGet: get end of data.\n");

			wsprintf(Lpw_temp,L"QTTSAudioGet: get end of data.");
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

			break;
		}
	}// while end 

	//����pcm�ļ�ͷ���ݵĴ�С
	default_pcmwavhdr.size_8 += default_pcmwavhdr.data_size + 36;

	//��������������д���ļ�ͷ��
	fseek(fp, 4, 0);
	fwrite(&default_pcmwavhdr.size_8,sizeof(default_pcmwavhdr.size_8), 1, fp);
	fseek(fp, 40, 0);
	fwrite(&default_pcmwavhdr.data_size,sizeof(default_pcmwavhdr.data_size), 1, fp);

	fclose(fp);

	ret = my_QTTSSessionEnd(sess_id, "");
	if ( ret != MSP_SUCCESS )
	{
		printf("QTTSSessionEnd: qtts end failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"QTTSSessionEnd: qtts end failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");
		my_QTTSFini();
		return ret;
	}

	/*********��ȡ�ս����ĻỰ����������Ϣ***********///�������Ҫ�ù��ܣ����ο���ɾ��
	// 	value_len = 32;//value_len���Ǵ�����������Ǵ���������ÿ�ε���QTTSGetParamʱҪ����Ϊbuffer����
	// 	ret = my_QTTSGetParam(sess_id,"upflow",param_value,&value_len);
	// 	if ( ret != MSP_SUCCESS )
	// 	{
	// 		printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
	// 
	// 		wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.", ret);
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");
	// 		my_QTTSFini();
	// 		return ret;
	// 	}
	// 	upload_flow = atoi(param_value);
	// 
	// 	printf("upload_flow_all== %d Byte\n",upload_flow);
	// 	wsprintf(Lpw_temp,L"upload_flow_all== %d Byte", upload_flow);
	// 	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 	value_len = 32;//value_len���Ǵ�����������Ǵ���������ÿ�ε���QTTSGetParamʱҪ����Ϊbuffer����
	// 	ret = my_QTTSGetParam(sess_id,"downflow",param_value,&value_len);
	// 	if ( ret != MSP_SUCCESS )
	// 	{
	// 		printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
	// 
	// 		wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.", ret);
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");
	// 		my_QTTSFini();
	// 		return ret;
	// 	}
	// 	download_flow = atoi(param_value);
	// 
	// 	printf("download_flow_all== %d Byte\n",download_flow);
	// 	wsprintf(Lpw_temp,L"download_flow_all== %d Byte", download_flow);
	// 	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

	/**************��ȡ������Ϣ����***************/
	ret = my_QTTSFini();
	if ( ret != MSP_SUCCESS )
	{
		printf("CMSPAudioCtrl::PlayText: qtts fini failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"CMSPAudioCtrl::PlayText: qtts fini failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");
		return ret;
	}

	wsprintf(Lpw_temp,L"TTSʶ�������");
	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

	Lpw_temp = NULL;
	delete[] Lpw_temp;


	return 0;
}

UINT CMyTxtToWavDlg::My_thread_TTS(LPVOID lpParam)
{
	LPWSTR	Lpw_temp =  new TCHAR[1024];
	TCHAR tchartemp[1024];

	//���洴��һ��WAV�ļ�������Ƶ��д��
	char* audio_data;
	unsigned int audio_len = 0;


	m_data * m_d = (m_data *)lpParam;

	const char*		str_MyAppId		= m_d->str_MyAppId;
	char*			str_Config		= m_d->str_Config;
	int				ret				;//= m_d->ret;
	const char*		sess_id			;//= m_d->sess_id;	
	int				synth_status	;//= m_d->synth_status;
	//���� �ָ�õ�txt����
	char*			str_Text		= m_d->m_chr_Txt;

	CMyTxtToWavDlg * m_MyTxtToWav	= m_d->m_MyTxtToWav;

	CListCtrl	   * m_listCtrl_Log ;

	if (m_d->iWhichThread == 1)
	{
		m_listCtrl_Log = m_d->mylistctrl;
	}
	else
	{
		m_listCtrl_Log = m_d->mylistctrl_2;
	}


	CButton		   * m_bt_StartTTS	= m_d->m_bt_StartTTS;

	const char*  tmpname = m_d->m_chr_WavPath;//"D:\\tts_audio_test.wav";

	CStatic		   * m_text_ShowNote = m_d->m_text_ShowNote;
	//	m_text_ShowNote->SetWindowTextW(L"�����ϳ�ing��������ص����������������ͨ����");

	m_listCtrl_Log->DeleteAllItems();
	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"��ʼ����TTS");


	//-----------------��ʼ��---------------------
	ret = QTTSInit( m_d->str_MyAppId);
	if ( ret != MSP_SUCCESS )
	{

		printf("QTTSInit: qtts init failed, ret = %d.\n", ret);

		wsprintf(Lpw_temp,L"QTTSInit: qtts init failed, ret = %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");

		return ret;
	}

	//���Σ� �Ựģʽ����Ƶ������������ѡ��
	sess_id = QTTSSessionBegin(m_d->str_Config, &ret);
	if ( ret != MSP_SUCCESS )
	{
		printf("QTTSSessionBegin: qtts begin session failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"QTTSSessionBegin: qtts begin session failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		if (ret > 0 && ret < 10100)
		{
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�������쳣�����Ժ����ԣ�");
		}

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");

		QTTSFini();
		return ret;
	}
	//-----------------��ʼ��end-------------------

	//������Ҫ�ϳɵ��ַ���
	ret = QTTSTextPut(sess_id, str_Text,strlen(str_Text) , NULL );//  sizeof(str_Text)

	if ( ret != MSP_SUCCESS )
	{
		printf("QTTSTextPut: qtts put text failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"QTTSTextPut: qtts put text failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");
		QTTSSessionEnd(sess_id, "");
		QTTSFini();
		return ret;
	}

	//���洴��һ��WAV�ļ�������Ƶ��д��
	FILE* fp = fopen(tmpname, "wb");
	if ( fp == NULL )
	{
		printf(" failed to open %s file.\n",tmpname);
		::MessageBox(NULL,L"����WAV��Ƶ�ļ�����!",L"error",0);


		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");

		QTTSSessionEnd(sess_id, "");
		QTTSFini();
		return ret;
	}

	fwrite(&default_pcmwavhdr, 1, sizeof(default_pcmwavhdr), fp);

	//------------------------------------------------------------

	int pos = 0;			//���ڱ����һ���Ѿ��ϳɵ���λ��
	int loop_count = 0;		//���ڱ�ǣ�ȡ�˼��ν��
	int upload_flow = 0,download_flow = 0;//�ϴ���������������
	char param_value[32] = "";//����ֵ���ַ�����ʽ
	size_t value_len = 32;	//�ַ������Ȼ�buffer����


	while ( true )
	{
		audio_data = (char*)QTTSAudioGet( sess_id ,&audio_len , &synth_status , &ret );
		/****************��ȡ�ϳ�λ��*************s****///�������Ҫ�ù��ܣ����ο���ɾ��
		const char* audio_info = QTTSAudioInfo(sess_id);	//��ȡ�Ѿ��õ�����Ƶ��λ��

		if (NULL == audio_info)
		{
			wsprintf(Lpw_temp,L"QTTSAudioInfo ��ȡ��Ƶλ����Ϣʧ��.");
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			continue;
		}


		int ced = atoi(audio_info+4);						//λ����Ϣ�ĸ�ʽ��ced=xxx������ǰ�ĸ��ַ����ض�����
		char text_complete[iAllLength];							//�����ַ��������Ѿ��ϳ���ϵ��ı���
		strcpy(text_complete,str_Text+pos);					//��ԭ�ַ������Ƶ����ַ����У�������ǰ�����нضϣ���֮ǰ�ϳɵģ��Ǳ��κϳɵ��ı�����
		text_complete[ced-pos]='\0';						//�����ַ������к󲿽ضϣ��ضϵ�ced��ʾ��λ�ã���������ַ������Ǳ��λ�ȡ�����ַ�����

		printf("[%d]:get result[err==%d / status==%d]:%s\n", (loop_count), ret, synth_status,text_complete);
		MultiByteToWideChar(CP_ACP,0,text_complete,-1,tchartemp,1024);
		wsprintf(Lpw_temp,L"[%d]:get result[err==%d / status==%d]:%s", (loop_count), ret, synth_status,tchartemp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		loop_count++;										//ȡ�����������
		pos = ced;											//�ϳ�λ�ø���
		/**************��ȡ�ϳ�λ�ý���***************/
		if ( ret != MSP_SUCCESS )
		{
			if (ret == MSP_ERROR_NO_MORE_DATA || ret ==  MSP_ERROR_NO_DATA || ret ==  MSP_ERROR_TTS_GENERAL)
			{
				//˵���Ѿ��������
				break;
			}
			printf("QTTSAudioGet: qtts get audio failed Error code %d.\n",ret);
			wsprintf(Lpw_temp,L"QTTSAudioGet: qtts get audio failed Error code %d.",ret);
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			//	char key = _getch();
			break;
		}
		/*********��ȡ��ǰ�Ự��ʱ��������Ϣ**********///�������Ҫ�ù��ܣ����ο���ɾ��
		value_len = 32;//value_len���Ǵ�����������Ǵ���������ÿ�ε���QTTSGetParamʱҪ����Ϊbuffer����
		ret = QTTSGetParam(sess_id,"upflow",param_value,&value_len);//��ȡ����������Ϣ
		if ( ret != MSP_SUCCESS )
		{
			printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
			wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.",ret);
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			break;
		}
		upload_flow = atoi(param_value);

		printf("upflow== %d Byte\n",upload_flow);
		wsprintf(Lpw_temp,L"upflow== %d Byte.",upload_flow);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		value_len = 32;//value_len���Ǵ�����������Ǵ���������ÿ�ε���QTTSGetParamʱҪ����Ϊbuffer����
		ret = QTTSGetParam(sess_id,"downflow",param_value,&value_len);//��ȡ����������Ϣ
		if ( ret != MSP_SUCCESS )
		{
			printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
			wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.\n",ret);
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			break;
		}
		download_flow = atoi(param_value);

		printf("downflow== %d Byte\n",download_flow);
		wsprintf(Lpw_temp,L"downflow== %d Byte",download_flow);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		/**************��ȡ������Ϣ����***************/
		fwrite(audio_data, 1, audio_len, fp);
		//	fwrite(audio_data, sizeof(audio_data), 1, fp);
		default_pcmwavhdr.data_size += audio_len;//����pcm���ݵĴ�С
		if ( MSP_TTS_FLAG_DATA_END == synth_status )
		{
			printf("QTTSAudioGet: get end of data.\n");

			wsprintf(Lpw_temp,L"QTTSAudioGet: get end of data.");
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

			break;
		}
	}// while end 

	//����pcm�ļ�ͷ���ݵĴ�С
	default_pcmwavhdr.size_8 += default_pcmwavhdr.data_size + 36;

	//��������������д���ļ�ͷ��
	fseek(fp, 4, 0);
	fwrite(&default_pcmwavhdr.size_8,sizeof(default_pcmwavhdr.size_8), 1, fp);
	fseek(fp, 40, 0);
	fwrite(&default_pcmwavhdr.data_size,sizeof(default_pcmwavhdr.data_size), 1, fp);

	fclose(fp);

	ret = QTTSSessionEnd(sess_id, "");
	if ( ret != MSP_SUCCESS )
	{
		printf("QTTSSessionEnd: qtts end failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"QTTSSessionEnd: qtts end failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");
		QTTSFini();
		return ret;
	}

	/*********��ȡ�ս����ĻỰ����������Ϣ***********///�������Ҫ�ù��ܣ����ο���ɾ��
	// 	value_len = 32;//value_len���Ǵ�����������Ǵ���������ÿ�ε���QTTSGetParamʱҪ����Ϊbuffer����
	// 	ret = QTTSGetParam(sess_id,"upflow",param_value,&value_len);
	// 	if ( ret != MSP_SUCCESS )
	// 	{
	// 		printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
	// 
	// 		wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.", ret);
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");
	// 		QTTSFini();
	// 		return ret;
	// 	}
	// 	upload_flow = atoi(param_value);
	// 
	// 	printf("upload_flow_all== %d Byte\n",upload_flow);
	// 	wsprintf(Lpw_temp,L"upload_flow_all== %d Byte", upload_flow);
	// 	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 	value_len = 32;//value_len���Ǵ�����������Ǵ���������ÿ�ε���QTTSGetParamʱҪ����Ϊbuffer����
	// 	ret = QTTSGetParam(sess_id,"downflow",param_value,&value_len);
	// 	if ( ret != MSP_SUCCESS )
	// 	{
	// 		printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
	// 
	// 		wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.", ret);
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");
	// 		QTTSFini();
	// 		return ret;
	// 	}
	// 	download_flow = atoi(param_value);
	// 
	// 	printf("download_flow_all== %d Byte\n",download_flow);
	// 	wsprintf(Lpw_temp,L"download_flow_all== %d Byte", download_flow);
	// 	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

	/**************��ȡ������Ϣ����***************/
	ret = QTTSFini();
	if ( ret != MSP_SUCCESS )
	{
		printf("CMSPAudioCtrl::PlayText: qtts fini failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"CMSPAudioCtrl::PlayText: qtts fini failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"�����ϳ�end");
		return ret;
	}

	wsprintf(Lpw_temp,L"TTSʶ�������");
	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

	Lpw_temp = NULL;
	delete[] Lpw_temp;


	return 0;
}

UINT CMyTxtToWavDlg::My_thread_All(LPVOID lpParam)
{

	m_data_ForAll *m_data_forall = (m_data_ForAll *)lpParam;

	int inn = 1;
	int iTxtAllLength = 0;
	int iYushu = 0;
	BOOL bJiOuShu = TRUE;//Yes Ϊ������NO ż��


	CStatic		   * m_text_ShowNote = m_data_forall->m_d_1.m_text_ShowNote;
	m_text_ShowNote->SetWindowTextW(L"�����ϳ�ing��������ص����������������ͨ����");



	//��ʼ��ȫ�ֱ���
	for (int ii = 0 ; ii < iAll ; ii++)
	{
		memset(charTxt[ii],0,iAllLength);
		m_needAgain->iStart  = 0;
		m_needAgain->iLength = 0;
	}

	CString *m_Str_TxtTemp		=	m_data_forall->m_d_1.m_str_TxtAll;
	CButton *m_bt_StartTTS		=	m_data_forall->m_d_1.m_bt_StartTTS;
	CComboBox *m_ctl_Combo_Informant = m_data_forall->m_d_1.m_ctl_Combo_Informant;

	//����ʼ��BT����
	m_bt_StartTTS->EnableWindow(0);
	m_ctl_Combo_Informant->EnableWindow(0);

	iTxtAllLength	= m_Str_TxtTemp->GetLength();
	iYushu			= iTxtAllLength % iFenGeShu;
	//�ж϶����TXT�ļ��ַ�������Ӣ�Ķ���һ���ַ���
	//һ������
	if (iTxtAllLength < iFenGeShu)
	{
		inn = 1;

		m_data_forall->m_d_1.iWhichThread = 1;
		CWinThread* pThread;

		USES_CONVERSION;
		sprintf(charTxt[0] ,"%s",T2A(*m_Str_TxtTemp) );

		m_data_forall->m_d_1.m_chr_Txt	= charTxt[0];
		strcat(m_data_forall->m_d_1.m_chr_WavPath , m_data_forall->m_d_1.m_chr_TxtFileName);
		//	m_d_1->m_chr_WavPath = 

		pThread = AfxBeginThread(My_thread_TTS, &m_data_forall->m_d_1,NULL,NULL,4);
		pThread->ResumeThread();

		HANDLE Handles;
		Handles = pThread->m_hThread;
		WaitForSingleObject(Handles,INFINITE);
		m_text_ShowNote->SetWindowTextW(L"�����ϳ�end~!�����ռǹ���ľ�д����������");
		m_ctl_Combo_Informant->EnableWindow(1);
		m_bt_StartTTS->EnableWindow(1);

	}
	//����һ������
	else
	{
		inn = iTxtAllLength / iFenGeShu;
		if (inn > iAll)
		{
			AfxMessageBox(L"���TxT�ļ�����̫���ˣ��뱣֤�����ļ���25W�����ڣ�");
			return 0;
		}

		int jj = 0;
		BOOL bA_Pass , bB_Pass;	//������ȳ�����λ�棬����Ϊ�١�
		bA_Pass = bB_Pass = FALSE;
		m_StrOut m_stroutTemp ,m_stroutTemp_2,m_stroutTemp_Temp;


		//����Ϊ inn  �����ֽ��� TTS��һ��ִ��2���߳�
		do
		{
			USES_CONVERSION;

			bA_Pass = bB_Pass = FALSE;

			if ( jj == 0)
			{
				//�ָ��ַ�
				m_stroutTemp = My_FenGe(*m_Str_TxtTemp,0,iFenGeShu);

				if (!m_stroutTemp.strOut.IsEmpty())
				{
					//���зָ���Ϣ�ı��棬�Է������ʱ�����ٴζ�ĳһ�δ������TTS
					m_needAgain[jj].iStart		= m_stroutTemp.iStartPot;
					m_needAgain[jj].iLength		= m_stroutTemp.iLength;
					//�ָ����ַ���
					sprintf(charTxt[jj] ,"%s",T2A(m_stroutTemp.strOut) );
					m_data_forall->m_d_1.m_chr_Txt			= charTxt[jj];
					m_data_forall->m_d_1.iWhichThread = 1;
					char * gghtemp = new char();
					sprintf(gghtemp,"%d",jj);
					strcpy(m_data_forall->m_d_1.m_chr_WavPath,	m_data_forall->m_d_1.m_chr_WavPath_Save);
					strcat(m_data_forall->m_d_1.m_chr_WavPath , m_data_forall->m_d_1.m_chr_TxtFileNoHouzuiming);
					strcat(m_data_forall->m_d_1.m_chr_WavPath , gghtemp);
					strcat(m_data_forall->m_d_1.m_chr_WavPath , ".wav");
					gghtemp = NULL;
					delete gghtemp;
					//--------------------------------------------------------
					m_stroutTemp_2.iNextStart = m_stroutTemp.iNextStart;
					//--------------------------------------------------------
				}
				else
				{
					bA_Pass = TRUE;
				}

				if (!bA_Pass)
				{
					//�ָ��ַ�
					m_stroutTemp_2 = My_FenGe(*m_Str_TxtTemp,m_stroutTemp_2.iNextStart,iFenGeShu);

					if (!m_stroutTemp_2.strOut.IsEmpty())
					{
						//���зָ���Ϣ�ı��棬�Է������ʱ�����ٴζ�ĳһ�δ������TTS
						m_needAgain[jj+1].iStart		= m_stroutTemp_2.iStartPot;
						m_needAgain[jj+1].iLength		= m_stroutTemp_2.iLength;
						//�ָ����ַ���
						sprintf(charTxt[jj+1] ,"%s",T2A(m_stroutTemp_2.strOut) );
						m_data_forall->m_d_2.m_chr_Txt			= charTxt[jj+1];
						m_data_forall->m_d_2.iWhichThread = 2;
						char * gghtemp_2 = new char();
						sprintf(gghtemp_2,"%d",jj+1);
						strcpy(m_data_forall->m_d_2.m_chr_WavPath,	m_data_forall->m_d_2.m_chr_WavPath_Save);
						strcat(m_data_forall->m_d_2.m_chr_WavPath , m_data_forall->m_d_2.m_chr_TxtFileNoHouzuiming);
						strcat(m_data_forall->m_d_2.m_chr_WavPath , gghtemp_2);
						strcat(m_data_forall->m_d_2.m_chr_WavPath , ".wav");
						gghtemp_2 = NULL;
						delete gghtemp_2;
						//--------------------------------------------------------
						m_stroutTemp_Temp.iNextStart = m_stroutTemp_2.iNextStart;
					}
					else
					{
						bB_Pass = TRUE;
					}
				}

			}
			else
			{
				m_stroutTemp.iNextStart = m_stroutTemp_Temp.iNextStart;
				//�ָ��ַ�
				m_stroutTemp = My_FenGe(*m_Str_TxtTemp,m_stroutTemp.iNextStart,iFenGeShu);

				if (!m_stroutTemp.strOut.IsEmpty())
				{
					//���зָ���Ϣ�ı��棬�Է������ʱ�����ٴζ�ĳһ�δ������TTS
					m_needAgain[jj].iStart		= m_stroutTemp.iStartPot;
					m_needAgain[jj].iLength		= m_stroutTemp.iLength;
					//�ָ����ַ���
					sprintf(charTxt[jj] ,"%s",T2A(m_stroutTemp.strOut) );
					m_data_forall->m_d_1.m_chr_Txt			= charTxt[jj];
					m_data_forall->m_d_1.iWhichThread = 1;
					char * gghtemp = new char();
					sprintf(gghtemp,"%d",jj);
					strcpy(m_data_forall->m_d_1.m_chr_WavPath,	m_data_forall->m_d_1.m_chr_WavPath_Save);
					strcat(m_data_forall->m_d_1.m_chr_WavPath , m_data_forall->m_d_1.m_chr_TxtFileNoHouzuiming);
					strcat(m_data_forall->m_d_1.m_chr_WavPath , gghtemp);
					strcat(m_data_forall->m_d_1.m_chr_WavPath , ".wav");
					gghtemp = NULL;
					delete gghtemp;
					//--------------------------------------------------------
					m_stroutTemp_2.iNextStart = m_stroutTemp.iNextStart;
					//--------------------------------------------------------
				}
				else
				{
					bA_Pass = bB_Pass = TRUE;
				}

				if (!bA_Pass)
				{
					//�ָ��ַ�
					m_stroutTemp_2 = My_FenGe(*m_Str_TxtTemp,m_stroutTemp_2.iNextStart,iFenGeShu);

					if (!m_stroutTemp_2.strOut.IsEmpty())
					{
						//���зָ���Ϣ�ı��棬�Է������ʱ�����ٴζ�ĳһ�δ������TTS
						m_needAgain[jj+1].iStart		= m_stroutTemp_2.iStartPot;
						m_needAgain[jj+1].iLength		= m_stroutTemp_2.iLength;
						//�ָ����ַ���
						sprintf(charTxt[jj+1] ,"%s",T2A(m_stroutTemp_2.strOut) );
						m_data_forall->m_d_2.m_chr_Txt			= charTxt[jj+1];
						m_data_forall->m_d_2.iWhichThread = 2;
						char * gghtemp_2 = new char();
						sprintf(gghtemp_2,"%d",jj+1);
						strcpy(m_data_forall->m_d_2.m_chr_WavPath,	m_data_forall->m_d_2.m_chr_WavPath_Save);
						strcat(m_data_forall->m_d_2.m_chr_WavPath , m_data_forall->m_d_2.m_chr_TxtFileNoHouzuiming);
						strcat(m_data_forall->m_d_2.m_chr_WavPath , gghtemp_2);
						strcat(m_data_forall->m_d_2.m_chr_WavPath , ".wav");
						gghtemp_2 = NULL;
						delete gghtemp_2;

						m_stroutTemp_Temp.iNextStart = m_stroutTemp_2.iNextStart;
					}
					else
					{
						bB_Pass = TRUE;
					}
				}
				//--------------------------------------------------------
			}

			jj += 2;

			CWinThread* pThread1;
			CWinThread* pThread2;
			HANDLE Handles[2];

			if (!bA_Pass)
			{
				pThread1 = AfxBeginThread(My_thread_TTS, &m_data_forall->m_d_1,NULL,NULL,4);//
				pThread1->ResumeThread();
				if (pThread1->m_hThread != NULL)
				{
					Handles[0]=pThread1->m_hThread;
				}
			}
			if (!bB_Pass)
			{
				pThread2 = AfxBeginThread(My_thread_TTS_2, &m_data_forall->m_d_2,NULL,NULL,4);//
				pThread2->ResumeThread();
				if (pThread2->m_hThread != NULL)
				{
					Handles[1]=pThread2->m_hThread;
				}
			}

			if (bA_Pass == FALSE && bB_Pass ==FALSE )
			{
				WaitForMultipleObjects(2,Handles,TRUE,INFINITE);
			}
			else if (bA_Pass == FALSE && bB_Pass == TRUE)
			{
				WaitForSingleObject(Handles[0],INFINITE);
			}

		} while (!bA_Pass && !bB_Pass);

		m_text_ShowNote->SetWindowTextW(L"�����ϳ�end���㶮�Ĺ�");
		m_ctl_Combo_Informant->EnableWindow(1);
		m_bt_StartTTS->EnableWindow(1);
	}

	return 716;
}

int CMyTxtToWavDlg::TTS_Start()
{
	CWinThread* pThread;
	UpdateData(TRUE);

	// 	(0,L"xiaoyan������Ů����");
	// 	(1,L"xiaoyu������������");
	// 	(2,L"Catherine��Ӣ��Ů����");
	// 	(3,L"henry��Ӣ��������	");
	// 	(4,L"vixm��С÷�����");
	// 	(5,L"vixl��С��̨����ͨ����");
	// 	(6,L"vixr��С�أ��Ĵ�����");
	// 	(7,L"vixyun��Сܿ����������");
	int index = m_ctl_Combo_Informant.GetCurSel();
	switch (index)
	{
	case 0:
		str_Config = "ssm=1,auf=audio/L16;rate=16000,vcn=xiaoyan";
		break;
	case 1:
		str_Config = "ssm=1,auf=audio/L16;rate=16000,vcn=xiaoyu";
		break;
	case 2:
		str_Config = "ssm=1,auf=audio/L16;rate=16000,ent=intp65_en,vcn=Catherine";
		break;
	case 3:
		str_Config = "ssm=1,auf=audio/L16;rate=16000,ent=intp65_en,vcn=henry";
		break;
	case 4:
		str_Config = "ssm=1,auf=audio/L16;rate=16000,ent=vivi21,vcn=vixm";
		break;
	case 5:
		str_Config = "ssm=1,auf=audio/L16;rate=16000,ent=vivi21,vcn=vixl";
		break;
	case 6:
		str_Config = "ssm=1,auf=audio/L16;rate=16000,ent=vivi21,vcn=vixr";
		break;
	case 7:
		str_Config = "ssm=1,auf=audio/L16;rate=16000,ent=vivi21,vcn=vixyun";
		break;
	}





	//���ӷ������ĳ�ʼ����Ϣ������ͳһ��ֵ
	m_d_1.str_Config		= str_Config;
	//	m_d_1.sess_id			= sess_id;
	m_d_1.ret				= ret;
	m_d_1.str_MyAppId		= str_MyAppId;
	//	m_d_1.synth_status		= synth_status;
	//Ȼ����  TXT ����������
	m_d_1.m_str_TxtAll		= &m_Str_TxtTemp;

	m_d_1.m_MyTxtToWav	= this;
	m_d_1.mylistctrl		= &m_listCtrl_Log;
	m_d_1.mylistctrl_2	= &m_listCtrl_Log2;
	m_d_1.m_bt_StartTTS	= &m_bt_StartTTS;
	m_d_1.m_text_ShowNote = &m_text_ShowNote;
	m_d_1.m_ctl_Combo_Informant = &m_ctl_Combo_Informant;
	//	m_d.m_str_TxtPart	= &m_Str_TxtTemp;
	m_d_1.m_str_TxtAll	= &m_Str_TxtTemp;
	//-------------------------------------
	m_d_2.str_Config		= str_Config;
	//	m_d_2.sess_id			= sess_id;
	m_d_2.ret				= ret;
	m_d_2.str_MyAppId		= str_MyAppId;
	//	m_d_2.synth_status		= synth_status;
	//Ȼ����  TXT ����������
	m_d_2.m_str_TxtAll		= &m_Str_TxtTemp;

	m_d_2.m_MyTxtToWav	= this;
	m_d_2.mylistctrl		= &m_listCtrl_Log;
	m_d_2.mylistctrl_2	= &m_listCtrl_Log2;
	m_d_2.m_bt_StartTTS	= &m_bt_StartTTS;
	m_d_2.m_text_ShowNote = &m_text_ShowNote;
	//	m_d.m_str_TxtPart	= &m_Str_TxtTemp;
	m_d_2.m_str_TxtAll	= &m_Str_TxtTemp;
	//-------------------------------------

	USES_CONVERSION;
	sprintf(chrtmep ,"%s",T2A(m_str_WavPath) );
	strcpy(m_d_1.m_chr_WavPath,chrtmep);
	strcpy(m_d_2.m_chr_WavPath,chrtmep);

	strcpy(m_d_1.m_chr_WavPath_Save,chrtmep);
	strcpy(m_d_2.m_chr_WavPath_Save,chrtmep);

	m_data_forall.m_d_1 = m_d_1;
	m_data_forall.m_d_2 = m_d_2;

	pThread = AfxBeginThread(My_thread_All, &m_data_forall,NULL,NULL,4);
	pThread->ResumeThread();

	return 716;
}

void CMyTxtToWavDlg::OnBnClickedBtTxt()
{
	// 	CString uuio = L"��ʲ���г���������¸¡��°�ʲ�������ң������ҿա���Ķ�";
	// 	m_StrOut m_stroutTemp,m_stroutTemp2,m_stroutTemp3, m_stroutTemp4;
	// 	m_stroutTemp = My_FenGe(uuio,0,13);
	// 	m_stroutTemp2 = My_FenGe(uuio,m_stroutTemp.iNextStart,13);
	// 	m_stroutTemp3 = My_FenGe(uuio,m_stroutTemp2.iNextStart,13);
	// 	m_stroutTemp4 = My_FenGe(uuio,m_stroutTemp3.iNextStart,13);
	// 
	// 	if (m_stroutTemp4.strOut.IsEmpty())
	// 	{
	// 		printf("123");
	// 	}
	// 	CString mnjh;
	// 
	// 	mnjh = uuio.Mid(m_stroutTemp.iStartPot,m_stroutTemp.iLength);
	// 	mnjh = uuio.Mid(m_stroutTemp2.iStartPot,m_stroutTemp2.iLength);
	// 	mnjh = uuio.Mid(m_stroutTemp3.iStartPot,m_stroutTemp3.iLength);

	CFileDialog dlg(TRUE, _T(DLG_EXT), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, _T(DLG_FILTER)); 

	if (dlg.DoModal() == IDOK) 
	{ 
		POSITION pos = dlg.GetStartPosition(); 

		while (pos != 0)
		{ 
			CString str_filePath = dlg.GetNextPathName(pos);
			m_edit_TxtfilePath.SetWindowTextW(str_filePath);
			m_bt_StartTTS.EnableWindow(TRUE);

			CString  hhtemp,jjtemp;
			int iFind = 0;
			wchar_t * wchrXie = L"\\";
			wchar_t * wchrDian = L".";

			iFind = str_filePath.ReverseFind(*wchrXie);

			hhtemp = str_filePath.Mid(iFind +1,str_filePath.GetLength());

			iFind = hhtemp.ReverseFind(*wchrDian);

			hhtemp = hhtemp.Mid(0,iFind);

			jjtemp = hhtemp;

			hhtemp += L".wav";

			USES_CONVERSION;

			sprintf(chrtmep ,"%s",T2A(hhtemp) );

			strcpy(m_d_1.m_chr_TxtFileName,chrtmep);

			strcpy(m_d_2.m_chr_TxtFileName,chrtmep);

			sprintf(chrtmep ,"%s",T2A(jjtemp) );

			strcpy(m_d_1.m_chr_TxtFileNoHouzuiming,chrtmep);

			strcpy(m_d_2.m_chr_TxtFileNoHouzuiming,chrtmep);
		}
	}
} 

void CMyTxtToWavDlg::OnBnClickedBtWavpath()
{
	BROWSEINFO stInfo = {NULL};
	LPCITEMIDLIST pIdlst;
	TCHAR szPath[MAX_PATH];
	stInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	stInfo.lpszTitle =  L"��ѡ��·��:";
	pIdlst = SHBrowseForFolder(&stInfo);
	if(!pIdlst) return;
	if(!SHGetPathFromIDList(pIdlst, szPath)) return;

	wcscat(szPath,L"\\");
	m_edit_WavPath.SetWindowTextW(szPath);
}

void CMyTxtToWavDlg::OnBnClickedBtOpenwavpath()
{
	CString lala;
	m_edit_WavPath.GetWindowTextW(lala);

	if (lala.IsEmpty())
	{
		MessageBox(L"����Wav��·������Ϊ���磡",L"Error",0);
		return;
	}

	ShellExecute(NULL,NULL,lala,//������������������Ҫ�򿪵��ļ�·��
		NULL,NULL,SW_SHOW);
}

void CMyTxtToWavDlg::OnBnClickedBtStarttts()
{

	// 	CString lala;
	// 	m_edit_TxtfilePath.GetWindowTextW(lala);
	// 
	// 	CFile file(lala,CFile::modeRead);
	// 	char *pBuf;
	// 	int iLen = file.GetLength();
	// 	pBuf = new char[iLen+1];
	// 	file.Read(pBuf,iLen);
	// 	pBuf[iLen]=0;
	// 	file.Close();
	// 
	// 	str_Text = pBuf;
	CString lala;

	m_edit_TxtfilePath.GetWindowTextW(lala);

	if (lala.IsEmpty())
	{
		MessageBox(L"��ѡ��һ��TXT�ļ���",L"Error",0);
		return;
	}

	try
	{
		CString lala;
		m_edit_TxtfilePath.GetWindowTextW(lala);
		CFile file(lala, CFile::modeRead);
		char* buf = NULL;
		DWORD dwLen = (DWORD)file.GetLength();
		buf = new char[dwLen + 1];
		memset(buf, 0, dwLen + 1);
		file.Read(buf, dwLen);
		file.Close();
		CString str(buf);
		delete[] buf;
		buf = NULL;
		m_Str_TxtTemp = str;
		//	int inut = 	str.GetLength();
		//	AfxMessageBox(str);
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}

	TTS_Start();
}

void CMyTxtToWavDlg::OnBnClickedCheckShowlog()
{
	if (m_ctl_Chech_ShowLog.GetCheck())
	{
		CRect rect;
		GetClientRect(&rect);
		rect.bottom = 665;
		rect.right = 645;
		AfxGetMainWnd()->MoveWindow(rect);
		AfxGetMainWnd()->CenterWindow();
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		rect.bottom = 160;
		rect.right = 645;
		AfxGetMainWnd()->MoveWindow(rect);
		AfxGetMainWnd()->CenterWindow();
	}
}

void CMyTxtToWavDlg::OnInsertitemListctrlLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	int nCount = m_listCtrl_Log.GetItemCount();
	if (nCount > 0)
		m_listCtrl_Log.EnsureVisible(nCount-1, FALSE);
}


void CMyTxtToWavDlg::OnInsertitemListctrlLog2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	int nCount = m_listCtrl_Log2.GetItemCount();
	if (nCount > 0)
		m_listCtrl_Log2.EnsureVisible(nCount-1, FALSE);
}
