
// MyTxtToWavDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTxtToWav.h"
#include "MyTxtToWavDlg.h"
#include "afxdialogex.h"

#define DLG_FILTER "txt Files (*.txt)|*.txt||" 
#define DLG_EXT   "* " 

//-------------------Wav 文件头------------------
#include "../include/wavehead.h"
//默认音频头部数据
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
//-------------------Wav 文件头end----------------


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyTxtToWavDlg 对话框
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

	str_Text  = "讯飞语音，沟通无限。";

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
//析构函数
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyTxtToWavDlg::OnPaint()
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
HCURSOR CMyTxtToWavDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// CMyTxtToWavDlg 消息处理程序

BOOL CMyTxtToWavDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//------------------------------------------------------------------------------------------------------------
	//生产dump文件
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


	//调整窗口大小，先隐藏 LOG 
	// 	CRect rect;
	// 	GetClientRect(&rect);
	// 	rect.left = 
	// 	rect.bottom = 160;
	// 	rect.right = 800;
	// 	AfxGetMainWnd()->MoveWindow(rect);
	// 	AfxGetMainWnd()->CenterWindow();

	m_listCtrl_Log.InsertColumn(0,L"输出信息",0,620);
	m_listCtrl_Log2.InsertColumn(0,L"输出信息",0,620);

	m_bt_StartTTS.EnableWindow(0);

	m_ctl_Combo_Informant.InsertString(0,L"xiaoyan（青年女声）");
	m_ctl_Combo_Informant.InsertString(1,L"xiaoyu（青年男声）");
	m_ctl_Combo_Informant.InsertString(2,L"Catherine（英文女声）");
	m_ctl_Combo_Informant.InsertString(3,L"henry（英文男声）	");
	m_ctl_Combo_Informant.InsertString(4,L"vixm（小梅，粤语）");
	m_ctl_Combo_Informant.InsertString(5,L"vixl（小莉，台湾普通话）");
	m_ctl_Combo_Informant.InsertString(6,L"vixr（小蓉，四川话）");
	m_ctl_Combo_Informant.InsertString(7,L"vixyun（小芸，东北话）");

	m_ctl_Combo_Informant.SetCurSel(0);

	InitializeFlatSB(m_hWnd);
	FlatSB_EnableScrollBar(m_hWnd, SB_BOTH, ESB_DISABLE_BOTH);

	//	TTS_Start();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
功能：
将传入的CString 进行切割，长度最好固定
返回值：
返回一个数组。m_StrOut  见定义
*/
m_StrOut CMyTxtToWavDlg::My_FenGe(CString poip, int istart, int ilength)
{
	m_StrOut m_strout;
	CString ioutstr , inowTemp;
	int iFind = 0;
	int iFind2 = 0;
	wchar_t * wchrDouhao = L"，";
	wchar_t * wchrJuhao = L"。";

	//先进行一个默认值的切割，有可能结尾不是'。'
	inowTemp = poip.Mid( istart, ilength);

	//然后进行反向的'。'的查找
	iFind  = inowTemp.ReverseFind(*wchrJuhao);
	iFind2 = inowTemp.ReverseFind(*wchrDouhao);

	//进行 。 位置的对比
	//当最后一位恰好是句号的时候
	if ( iFind ==  ilength - 1)
	{
		m_strout.iNextStart = istart + ilength;
		m_strout.strOut = inowTemp;
		m_strout.iStartPot = istart;
		m_strout.iLength = inowTemp.GetLength();
	}
	//当最后一位恰好是逗号的时候
	else if (iFind2 == ilength - 1)
	{
		m_strout.iNextStart = istart + ilength;
		m_strout.strOut = inowTemp;
		m_strout.iStartPot = istart;
		m_strout.iLength = inowTemp.GetLength();
	}
	//如果 句号 在 逗号 后面，那么以句号为截断末尾
	else if (iFind > iFind2)
	{
		m_strout.iNextStart = istart + iFind + 1;
		m_strout.strOut = poip.Mid( istart, iFind + 1);
		m_strout.iStartPot = istart;
		m_strout.iLength = m_strout.strOut.GetLength();
	}
	//如果 句号 在 逗号 后面，那么以句号为截断末尾
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

	//获取message函数的指针
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

	//下面创建一个WAV文件用于音频的写入
	char* audio_data;
	unsigned int audio_len = 0;


	m_data * m_d = (m_data *)lpParam;

	const char*		str_MyAppId		= m_d->str_MyAppId;
	char*			str_Config		= m_d->str_Config;
	int				ret				;//= m_d->ret;
	const char*		sess_id			;//= m_d->sess_id;	
	int				synth_status	;//= m_d->synth_status;
	//传入 分割好的txt内容
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
	//	m_text_ShowNote->SetWindowTextW(L"语音合成ing···别关掉程序哈！保持网络通畅！");

	m_listCtrl_Log->DeleteAllItems();
	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"开始进行TTS");


	//-----------------初始化---------------------

	ret = my_QTTSInit( m_d->str_MyAppId );
	//	ret = QTTSInit( m_d->str_MyAppId);
	if ( ret != MSP_SUCCESS )
	{

		printf("QTTSInit: qtts init failed, ret = %d.\n", ret);

		wsprintf(Lpw_temp,L"QTTSInit: qtts init failed, ret = %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");

		return ret;
	}

	//传参： 会话模式、音频质量、发音人选择
	sess_id = my_QTTSSessionBegin(m_d->str_Config, &ret);
	if ( ret != MSP_SUCCESS )
	{
		printf("QTTSSessionBegin: qtts begin session failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"QTTSSessionBegin: qtts begin session failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		if (ret > 0 && ret < 10100)
		{
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"服务器异常，请稍后再试！");
		}

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");

		my_QTTSFini();
		return ret;
	}
	//-----------------初始化end-------------------

	//发送需要合成的字符串
	ret = my_QTTSTextPut(sess_id, str_Text,strlen(str_Text) , NULL );//  sizeof(str_Text)

	if ( ret != MSP_SUCCESS )
	{
		printf("QTTSTextPut: qtts put text failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"QTTSTextPut: qtts put text failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");
		my_QTTSSessionEnd(sess_id, "");
		my_QTTSFini();
		return ret;
	}

	//下面创建一个WAV文件用于音频的写入
	FILE* fp = fopen(tmpname, "wb");
	if ( fp == NULL )
	{
		printf(" failed to open %s file.\n",tmpname);
		::MessageBox(NULL,L"创建WAV音频文件出错!",L"error",0);


		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");

		my_QTTSSessionEnd(sess_id, "");
		my_QTTSFini();
		return ret;
	}

	fwrite(&default_pcmwavhdr, 1, sizeof(default_pcmwavhdr), fp);

	//------------------------------------------------------------

	int pos = 0;			//用于标记上一次已经合成到的位置
	int loop_count = 0;		//用于标记，取了几次结果
	int upload_flow = 0,download_flow = 0;//上传流量和下载流量
	char param_value[32] = "";//参数值的字符串形式
	size_t value_len = 32;	//字符串长度或buffer长度


	while ( true )
	{
		audio_data = (char*)my_QTTSAudioGet( sess_id ,&audio_len , &synth_status , &ret );
		/****************获取合成位置*************s****///如果不需要该功能，本段可以删除
		const char* audio_info = my_QTTSAudioInfo(sess_id);	//获取已经得到的音频的位置

		if (NULL == audio_info)
		{
			wsprintf(Lpw_temp,L"QTTSAudioInfo 获取音频位置信息失败.");
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			continue;
		}


		int ced = atoi(audio_info+4);						//位置信息的格式是ced=xxx，所以前四个字符被截断抛弃
		char text_complete[iAllLength];							//声明字符串，“已经合成完毕的文本”
		strcpy(text_complete,str_Text+pos);					//将原字符串复制到新字符串中，并对其前部进行截断，把之前合成的，非本次合成的文本抛弃
		text_complete[ced-pos]='\0';						//将新字符串进行后部截断，截断到ced提示的位置，这样这个字符串就是本次获取到的字符串了

		printf("[%d]:get result[err==%d / status==%d]:%s\n", (loop_count), ret, synth_status,text_complete);
		MultiByteToWideChar(CP_ACP,0,text_complete,-1,tchartemp,1024);
		wsprintf(Lpw_temp,L"[%d]:get result[err==%d / status==%d]:%s", (loop_count), ret, synth_status,tchartemp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		loop_count++;										//取结果次数自增
		pos = ced;											//合成位置更新
		/**************获取合成位置结束***************/
		if ( ret != MSP_SUCCESS )
		{
			if (ret == MSP_ERROR_NO_MORE_DATA || ret ==  MSP_ERROR_NO_DATA || ret ==  MSP_ERROR_TTS_GENERAL)
			{
				//说明已经接受完毕
				break;
			}
			printf("QTTSAudioGet: qtts get audio failed Error code %d.\n",ret);
			wsprintf(Lpw_temp,L"QTTSAudioGet: qtts get audio failed Error code %d.",ret);
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			//	char key = _getch();
			break;
		}
		/*********获取当前会话此时的流量信息**********///如果不需要该功能，本段可以删除
		value_len = 32;//value_len既是传入参数，又是传出参数，每次调用QTTSGetParam时要调整为buffer长度
		ret = my_QTTSGetParam(sess_id,"upflow",param_value,&value_len);//获取上行流量信息
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

		value_len = 32;//value_len既是传入参数，又是传出参数，每次调用QTTSGetParam时要调整为buffer长度
		ret = my_QTTSGetParam(sess_id,"downflow",param_value,&value_len);//获取下行流量信息
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

		/**************获取流量信息结束***************/
		fwrite(audio_data, 1, audio_len, fp);
		//	fwrite(audio_data, sizeof(audio_data), 1, fp);
		default_pcmwavhdr.data_size += audio_len;//修正pcm数据的大小
		if ( MSP_TTS_FLAG_DATA_END == synth_status )
		{
			printf("QTTSAudioGet: get end of data.\n");

			wsprintf(Lpw_temp,L"QTTSAudioGet: get end of data.");
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

			break;
		}
	}// while end 

	//修正pcm文件头数据的大小
	default_pcmwavhdr.size_8 += default_pcmwavhdr.data_size + 36;

	//将修正过的数据写回文件头部
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

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");
		my_QTTSFini();
		return ret;
	}

	/*********获取刚结束的会话流量总量信息***********///如果不需要该功能，本段可以删除
	// 	value_len = 32;//value_len既是传入参数，又是传出参数，每次调用QTTSGetParam时要调整为buffer长度
	// 	ret = my_QTTSGetParam(sess_id,"upflow",param_value,&value_len);
	// 	if ( ret != MSP_SUCCESS )
	// 	{
	// 		printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
	// 
	// 		wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.", ret);
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");
	// 		my_QTTSFini();
	// 		return ret;
	// 	}
	// 	upload_flow = atoi(param_value);
	// 
	// 	printf("upload_flow_all== %d Byte\n",upload_flow);
	// 	wsprintf(Lpw_temp,L"upload_flow_all== %d Byte", upload_flow);
	// 	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 	value_len = 32;//value_len既是传入参数，又是传出参数，每次调用QTTSGetParam时要调整为buffer长度
	// 	ret = my_QTTSGetParam(sess_id,"downflow",param_value,&value_len);
	// 	if ( ret != MSP_SUCCESS )
	// 	{
	// 		printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
	// 
	// 		wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.", ret);
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");
	// 		my_QTTSFini();
	// 		return ret;
	// 	}
	// 	download_flow = atoi(param_value);
	// 
	// 	printf("download_flow_all== %d Byte\n",download_flow);
	// 	wsprintf(Lpw_temp,L"download_flow_all== %d Byte", download_flow);
	// 	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

	/**************获取流量信息结束***************/
	ret = my_QTTSFini();
	if ( ret != MSP_SUCCESS )
	{
		printf("CMSPAudioCtrl::PlayText: qtts fini failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"CMSPAudioCtrl::PlayText: qtts fini failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");
		return ret;
	}

	wsprintf(Lpw_temp,L"TTS识别结束。");
	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

	Lpw_temp = NULL;
	delete[] Lpw_temp;


	return 0;
}

UINT CMyTxtToWavDlg::My_thread_TTS(LPVOID lpParam)
{
	LPWSTR	Lpw_temp =  new TCHAR[1024];
	TCHAR tchartemp[1024];

	//下面创建一个WAV文件用于音频的写入
	char* audio_data;
	unsigned int audio_len = 0;


	m_data * m_d = (m_data *)lpParam;

	const char*		str_MyAppId		= m_d->str_MyAppId;
	char*			str_Config		= m_d->str_Config;
	int				ret				;//= m_d->ret;
	const char*		sess_id			;//= m_d->sess_id;	
	int				synth_status	;//= m_d->synth_status;
	//传入 分割好的txt内容
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
	//	m_text_ShowNote->SetWindowTextW(L"语音合成ing···别关掉程序哈！保持网络通畅！");

	m_listCtrl_Log->DeleteAllItems();
	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"开始进行TTS");


	//-----------------初始化---------------------
	ret = QTTSInit( m_d->str_MyAppId);
	if ( ret != MSP_SUCCESS )
	{

		printf("QTTSInit: qtts init failed, ret = %d.\n", ret);

		wsprintf(Lpw_temp,L"QTTSInit: qtts init failed, ret = %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");

		return ret;
	}

	//传参： 会话模式、音频质量、发音人选择
	sess_id = QTTSSessionBegin(m_d->str_Config, &ret);
	if ( ret != MSP_SUCCESS )
	{
		printf("QTTSSessionBegin: qtts begin session failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"QTTSSessionBegin: qtts begin session failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		if (ret > 0 && ret < 10100)
		{
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"服务器异常，请稍后再试！");
		}

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");

		QTTSFini();
		return ret;
	}
	//-----------------初始化end-------------------

	//发送需要合成的字符串
	ret = QTTSTextPut(sess_id, str_Text,strlen(str_Text) , NULL );//  sizeof(str_Text)

	if ( ret != MSP_SUCCESS )
	{
		printf("QTTSTextPut: qtts put text failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"QTTSTextPut: qtts put text failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");
		QTTSSessionEnd(sess_id, "");
		QTTSFini();
		return ret;
	}

	//下面创建一个WAV文件用于音频的写入
	FILE* fp = fopen(tmpname, "wb");
	if ( fp == NULL )
	{
		printf(" failed to open %s file.\n",tmpname);
		::MessageBox(NULL,L"创建WAV音频文件出错!",L"error",0);


		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");

		QTTSSessionEnd(sess_id, "");
		QTTSFini();
		return ret;
	}

	fwrite(&default_pcmwavhdr, 1, sizeof(default_pcmwavhdr), fp);

	//------------------------------------------------------------

	int pos = 0;			//用于标记上一次已经合成到的位置
	int loop_count = 0;		//用于标记，取了几次结果
	int upload_flow = 0,download_flow = 0;//上传流量和下载流量
	char param_value[32] = "";//参数值的字符串形式
	size_t value_len = 32;	//字符串长度或buffer长度


	while ( true )
	{
		audio_data = (char*)QTTSAudioGet( sess_id ,&audio_len , &synth_status , &ret );
		/****************获取合成位置*************s****///如果不需要该功能，本段可以删除
		const char* audio_info = QTTSAudioInfo(sess_id);	//获取已经得到的音频的位置

		if (NULL == audio_info)
		{
			wsprintf(Lpw_temp,L"QTTSAudioInfo 获取音频位置信息失败.");
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			continue;
		}


		int ced = atoi(audio_info+4);						//位置信息的格式是ced=xxx，所以前四个字符被截断抛弃
		char text_complete[iAllLength];							//声明字符串，“已经合成完毕的文本”
		strcpy(text_complete,str_Text+pos);					//将原字符串复制到新字符串中，并对其前部进行截断，把之前合成的，非本次合成的文本抛弃
		text_complete[ced-pos]='\0';						//将新字符串进行后部截断，截断到ced提示的位置，这样这个字符串就是本次获取到的字符串了

		printf("[%d]:get result[err==%d / status==%d]:%s\n", (loop_count), ret, synth_status,text_complete);
		MultiByteToWideChar(CP_ACP,0,text_complete,-1,tchartemp,1024);
		wsprintf(Lpw_temp,L"[%d]:get result[err==%d / status==%d]:%s", (loop_count), ret, synth_status,tchartemp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

		loop_count++;										//取结果次数自增
		pos = ced;											//合成位置更新
		/**************获取合成位置结束***************/
		if ( ret != MSP_SUCCESS )
		{
			if (ret == MSP_ERROR_NO_MORE_DATA || ret ==  MSP_ERROR_NO_DATA || ret ==  MSP_ERROR_TTS_GENERAL)
			{
				//说明已经接受完毕
				break;
			}
			printf("QTTSAudioGet: qtts get audio failed Error code %d.\n",ret);
			wsprintf(Lpw_temp,L"QTTSAudioGet: qtts get audio failed Error code %d.",ret);
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
			//	char key = _getch();
			break;
		}
		/*********获取当前会话此时的流量信息**********///如果不需要该功能，本段可以删除
		value_len = 32;//value_len既是传入参数，又是传出参数，每次调用QTTSGetParam时要调整为buffer长度
		ret = QTTSGetParam(sess_id,"upflow",param_value,&value_len);//获取上行流量信息
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

		value_len = 32;//value_len既是传入参数，又是传出参数，每次调用QTTSGetParam时要调整为buffer长度
		ret = QTTSGetParam(sess_id,"downflow",param_value,&value_len);//获取下行流量信息
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

		/**************获取流量信息结束***************/
		fwrite(audio_data, 1, audio_len, fp);
		//	fwrite(audio_data, sizeof(audio_data), 1, fp);
		default_pcmwavhdr.data_size += audio_len;//修正pcm数据的大小
		if ( MSP_TTS_FLAG_DATA_END == synth_status )
		{
			printf("QTTSAudioGet: get end of data.\n");

			wsprintf(Lpw_temp,L"QTTSAudioGet: get end of data.");
			m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

			break;
		}
	}// while end 

	//修正pcm文件头数据的大小
	default_pcmwavhdr.size_8 += default_pcmwavhdr.data_size + 36;

	//将修正过的数据写回文件头部
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

		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");
		QTTSFini();
		return ret;
	}

	/*********获取刚结束的会话流量总量信息***********///如果不需要该功能，本段可以删除
	// 	value_len = 32;//value_len既是传入参数，又是传出参数，每次调用QTTSGetParam时要调整为buffer长度
	// 	ret = QTTSGetParam(sess_id,"upflow",param_value,&value_len);
	// 	if ( ret != MSP_SUCCESS )
	// 	{
	// 		printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
	// 
	// 		wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.", ret);
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");
	// 		QTTSFini();
	// 		return ret;
	// 	}
	// 	upload_flow = atoi(param_value);
	// 
	// 	printf("upload_flow_all== %d Byte\n",upload_flow);
	// 	wsprintf(Lpw_temp,L"upload_flow_all== %d Byte", upload_flow);
	// 	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 	value_len = 32;//value_len既是传入参数，又是传出参数，每次调用QTTSGetParam时要调整为buffer长度
	// 	ret = QTTSGetParam(sess_id,"downflow",param_value,&value_len);
	// 	if ( ret != MSP_SUCCESS )
	// 	{
	// 		printf("QTTSGetParam: qtts get param failed Error code %d.\n",ret);
	// 
	// 		wsprintf(Lpw_temp,L"QTTSGetParam: qtts get param failed Error code %d.", ret);
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
	// 
	// 		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");
	// 		QTTSFini();
	// 		return ret;
	// 	}
	// 	download_flow = atoi(param_value);
	// 
	// 	printf("download_flow_all== %d Byte\n",download_flow);
	// 	wsprintf(Lpw_temp,L"download_flow_all== %d Byte", download_flow);
	// 	m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);

	/**************获取流量信息结束***************/
	ret = QTTSFini();
	if ( ret != MSP_SUCCESS )
	{
		printf("CMSPAudioCtrl::PlayText: qtts fini failed Error code %d.\n",ret);

		wsprintf(Lpw_temp,L"CMSPAudioCtrl::PlayText: qtts fini failed Error code %d.", ret);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,Lpw_temp);
		m_listCtrl_Log->InsertItem(m_listCtrl_Log->GetItemCount()+1,L"语音合成end");
		return ret;
	}

	wsprintf(Lpw_temp,L"TTS识别结束。");
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
	BOOL bJiOuShu = TRUE;//Yes 为奇数，NO 偶数


	CStatic		   * m_text_ShowNote = m_data_forall->m_d_1.m_text_ShowNote;
	m_text_ShowNote->SetWindowTextW(L"语音合成ing···别关掉程序哈！保持网络通畅！");



	//初始化全局变量
	for (int ii = 0 ; ii < iAll ; ii++)
	{
		memset(charTxt[ii],0,iAllLength);
		m_needAgain->iStart  = 0;
		m_needAgain->iLength = 0;
	}

	CString *m_Str_TxtTemp		=	m_data_forall->m_d_1.m_str_TxtAll;
	CButton *m_bt_StartTTS		=	m_data_forall->m_d_1.m_bt_StartTTS;
	CComboBox *m_ctl_Combo_Informant = m_data_forall->m_d_1.m_ctl_Combo_Informant;

	//将开始的BT禁用
	m_bt_StartTTS->EnableWindow(0);
	m_ctl_Combo_Informant->EnableWindow(0);

	iTxtAllLength	= m_Str_TxtTemp->GetLength();
	iYushu			= iTxtAllLength % iFenGeShu;
	//判断读入的TXT文件字符数（中英文都算一个字符）
	//一个部分
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
		m_text_ShowNote->SetWindowTextW(L"语音合成end~!看看日记哈，木有错误就完美了");
		m_ctl_Combo_Informant->EnableWindow(1);
		m_bt_StartTTS->EnableWindow(1);

	}
	//超过一个部分
	else
	{
		inn = iTxtAllLength / iFenGeShu;
		if (inn > iAll)
		{
			AfxMessageBox(L"这个TxT文件字数太多了，请保证单个文件在25W字以内！");
			return 0;
		}

		int jj = 0;
		BOOL bA_Pass , bB_Pass;	//如果长度超过就位真，否则为假。
		bA_Pass = bB_Pass = FALSE;
		m_StrOut m_stroutTemp ,m_stroutTemp_2,m_stroutTemp_Temp;


		//将分为 inn  个部分进行 TTS，一次执行2个线程
		do
		{
			USES_CONVERSION;

			bA_Pass = bB_Pass = FALSE;

			if ( jj == 0)
			{
				//分割字符
				m_stroutTemp = My_FenGe(*m_Str_TxtTemp,0,iFenGeShu);

				if (!m_stroutTemp.strOut.IsEmpty())
				{
					//进行分割信息的保存，以防出错的时候能再次对某一段错误进行TTS
					m_needAgain[jj].iStart		= m_stroutTemp.iStartPot;
					m_needAgain[jj].iLength		= m_stroutTemp.iLength;
					//分割后的字符串
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
					//分割字符
					m_stroutTemp_2 = My_FenGe(*m_Str_TxtTemp,m_stroutTemp_2.iNextStart,iFenGeShu);

					if (!m_stroutTemp_2.strOut.IsEmpty())
					{
						//进行分割信息的保存，以防出错的时候能再次对某一段错误进行TTS
						m_needAgain[jj+1].iStart		= m_stroutTemp_2.iStartPot;
						m_needAgain[jj+1].iLength		= m_stroutTemp_2.iLength;
						//分割后的字符串
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
				//分割字符
				m_stroutTemp = My_FenGe(*m_Str_TxtTemp,m_stroutTemp.iNextStart,iFenGeShu);

				if (!m_stroutTemp.strOut.IsEmpty())
				{
					//进行分割信息的保存，以防出错的时候能再次对某一段错误进行TTS
					m_needAgain[jj].iStart		= m_stroutTemp.iStartPot;
					m_needAgain[jj].iLength		= m_stroutTemp.iLength;
					//分割后的字符串
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
					//分割字符
					m_stroutTemp_2 = My_FenGe(*m_Str_TxtTemp,m_stroutTemp_2.iNextStart,iFenGeShu);

					if (!m_stroutTemp_2.strOut.IsEmpty())
					{
						//进行分割信息的保存，以防出错的时候能再次对某一段错误进行TTS
						m_needAgain[jj+1].iStart		= m_stroutTemp_2.iStartPot;
						m_needAgain[jj+1].iLength		= m_stroutTemp_2.iLength;
						//分割后的字符串
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

		m_text_ShowNote->SetWindowTextW(L"语音合成end！你懂的哈");
		m_ctl_Combo_Informant->EnableWindow(1);
		m_bt_StartTTS->EnableWindow(1);
	}

	return 716;
}

int CMyTxtToWavDlg::TTS_Start()
{
	CWinThread* pThread;
	UpdateData(TRUE);

	// 	(0,L"xiaoyan（青年女声）");
	// 	(1,L"xiaoyu（青年男声）");
	// 	(2,L"Catherine（英文女声）");
	// 	(3,L"henry（英文男声）	");
	// 	(4,L"vixm（小梅，粤语）");
	// 	(5,L"vixl（小莉，台湾普通话）");
	// 	(6,L"vixr（小蓉，四川话）");
	// 	(7,L"vixyun（小芸，东北话）");
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





	//连接服务器的初始化信息，可以统一赋值
	m_d_1.str_Config		= str_Config;
	//	m_d_1.sess_id			= sess_id;
	m_d_1.ret				= ret;
	m_d_1.str_MyAppId		= str_MyAppId;
	//	m_d_1.synth_status		= synth_status;
	//然后传入  TXT 的所有内容
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
	//然后传入  TXT 的所有内容
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
	// 	CString uuio = L"阿什自行车，跟打算嘎嘎。嘎阿什顿请问我，人我我空。间的额";
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
	stInfo.lpszTitle =  L"请选择路径:";
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
		MessageBox(L"保存Wav的路径不能为空噻！",L"Error",0);
		return;
	}

	ShellExecute(NULL,NULL,lala,//第三个参数，输入你要打开的文件路径
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
		MessageBox(L"请选择一个TXT文件！",L"Error",0);
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int nCount = m_listCtrl_Log.GetItemCount();
	if (nCount > 0)
		m_listCtrl_Log.EnsureVisible(nCount-1, FALSE);
}


void CMyTxtToWavDlg::OnInsertitemListctrlLog2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int nCount = m_listCtrl_Log2.GetItemCount();
	if (nCount > 0)
		m_listCtrl_Log2.EnsureVisible(nCount-1, FALSE);
}
