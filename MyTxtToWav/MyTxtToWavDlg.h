
// MyTxtToWavDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "../include/Timer.h"
#include "../include/MiniDump.h"

//-------------------------------------------------------------------------------
//这里重新定义msc.dll 的导出函数，为了开启另一个线程

//		int MSPAPI QTTSInit(const char* configs)
typedef int (WINAPI My_QTTSInit)(const char* configs);

//MSPAPI const char* QTTSSessionBegin(const char* params, int* errorCode)
typedef const char* (WINAPI My_QTTSSessionBegin)(const char* params, int* errorCode);

//		int MSPAPI QTTSTextPut(const char* sessionID, const char* textString, unsigned int textLen, const char* params)
typedef int (WINAPI My_QTTSTextPut)(const char* sessionID, const char* textString, unsigned int textLen, const char* params);

//const void* MSPAPI QTTSAudioGet(const char* sessionID, unsigned int* audioLen, int* synthStatus, int* errorCode)
typedef void* (WINAPI My_QTTSAudioGet)(const char* sessionID, unsigned int* audioLen, int* synthStatus, int* errorCode);

//		int MSPAPI QTTSGetParam(const char* sessionID, const char* paramName, char* paramValue, unsigned int* valueLen)
typedef int (WINAPI My_QTTSGetParam)(const char* sessionID, const char* paramName, char* paramValue, unsigned int* valueLen);

//		int MSPAPI QTTSSessionEnd(const char* sessionID, const char* hints)
typedef int (WINAPI My_QTTSSessionEnd)(const char* sessionID, const char* hints);

//int MSPAPI QTTSFini(void)
typedef int (WINAPI My_QTTSFini)(void);

//const char* MSPAPI QTTSAudioInfo(const char* sessionID)
typedef const char* (WINAPI My_QTTSAudioInfo)(const char* sessionID);

//-------------------------------------------------------------------------------


static CTimer * My_timer = new CTimer();
static float	fTimeOfAll;		//TTS总需要花的时间

static BOOL  bRun;
static int	 iFenGeShu = 2000;//2000;//分割的大小 ，这个看情况。******************************************************
							//如果配置文件的上传文本默认长度是  1024 那么就设置 500 算汉字
							//最高上传的长度可以到 4096 那么也可以相应的调整到 2000 算汉字

static	const int  iAll = 128;//允许最大的分割数，可以分成128组，当然分成120组就差不多了
static	const int  iAllLength = 4096;


struct m_StrOut				//用于输出分割字符串后信息的结构体
{
	CString		strOut;		//输出的字符串
	int			iNextStart; //下一个开始的位置
	int			iStartPot;	//输出的这个字符串的开始位置
	int			iLength;	//输出的这个字符串的结束位置
	//后面这两个是用来输出，给数组保存，以为这一段 TTS 失败，下次重新再做的。
};

struct m_NeedAgain		//用于存储TTS失败的字符串信息，方便进行再次TTS
{
	int		iStart;
	int		iLength;
};


										//这里的几个 512 只不过是 限制 一下 每个txt的最大字数。
										//比如 512 ， 每个长度在 4096 的最大 字数在 512 * 2000 = 1024000
										//当然灰常的不建议一次去 TTS 那么多文字，真的很要时间的哇
						
										//charTxt[128][4096]  intTxt_start[512];   intTxt_end[512];
						
										//默认还是不给那么大的，charTxt[128][4096]  就好了，单个上传最大，然后总字数将一定的限制

static char charTxt[iAll][iAllLength];	//分割的字符串存放  ，这里的 4096 或者是 1024 参见 iFenGeShu 的说明

static m_NeedAgain m_needAgain[iAll];  //用于存储TTS失败的字符串信息，方便进行再次TTS



// CMyTxtToWavDlg 对话框
class CMyTxtToWavDlg : public CDialogEx
{
	//传递参数
	struct m_data 
	{
		const char*		str_MyAppId;
		char*			str_Config;
		int				ret;
	//	const char*		sess_id;	
	//	int				synth_status;
		char*			m_chr_WavPath;//存储wav的路径文件名，最后合成
		char*			m_chr_WavPath_Save;//存储wav的路径，用来保存 D:\ 不带文件名
		char*			m_chr_Txt;

		int				iWhichThread;

		char*			m_chr_TxtFileName;
		char*			m_chr_TxtFileNoHouzuiming;//没得后罪名

		CMyTxtToWavDlg*	m_MyTxtToWav;
		CListCtrl *		mylistctrl;
		CListCtrl *		mylistctrl_2;

		CButton   *		m_bt_StartTTS;
		CStatic	  *		m_text_ShowNote;

		CComboBox *		m_ctl_Combo_Informant;

		CString	 *		m_str_TxtPart;//分割好的一部分
		CString  *		m_str_TxtAll;//全部的字符串
		
	};
	//一次传2个进去，就不会因为指针乱了
	struct m_data_ForAll
	{
		m_data m_d_1;
		m_data m_d_2;
	};

// 构造
public:
	CMyTxtToWavDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMyTxtToWavDlg();

	//----------------------------------------------------
private:
	char *chrtmep;
	void My_DelLog();
public:							

	m_data				m_d_1, m_d_2;				//传递参数
	m_data_ForAll		m_data_forall;

	const char* str_MyAppId;//APPID请勿随意改动
	char* str_Config;		//传参
	int ret;				//网络传输返回值
	CString		m_Str_TxtTemp;//临时储存TXT的全部内容





/*
	MSP_TTS_FLAG_STILL_HAVE_DATA = 1	音频还没取完，还有后继的音频

	MSP_TTS_FLAG_DATA_END = 2			音频已经取完

	MSP_TTS_FLAG_CMD_CANCELED = 3		保留
*/
	//音频还没取完，还有后继的音频
	int synth_status;
	char* str_Text;

	//MSC为本路会话建立的ID，用来唯一的标识本路会话，供以后调用其他函数时使用。函数调用失败则会返回NULL。
//	char* sess_id;	

public:
	int TTS_Start();	//开始TTS识别

	static m_StrOut My_FenGe(CString poip, int istart, int ilength);

public:
	static UINT My_thread_TTS(LPVOID lpParam);	//默认一个线程就是调用这个啦

	static UINT My_thread_TTS_2(LPVOID lpParam);//用到另一个 DLL，DLL是同一个，只不过位置不同，变向实现多线程

	static UINT My_thread_All(LPVOID lpParam);


	//----------------------------------------------------
// 对话框数据
	enum { IDD = IDD_MYTXTTOWAV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtTxt();
	CEdit m_edit_TxtfilePath;
	afx_msg void OnBnClickedBtWavpath();
	CEdit m_edit_WavPath;
	afx_msg void OnBnClickedBtOpenwavpath();
	afx_msg void OnBnClickedBtStarttts();
	CListCtrl m_listCtrl_Log;
	CButton m_bt_StartTTS;
	CStatic m_text_ShowNote;
	CString m_str_WavPath;
	afx_msg void OnInsertitemListctrlLog(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listCtrl_Log2;
	CButton m_ctl_Chech_ShowLog;
	BOOL m_b_Check_ShowLog;
	afx_msg void OnBnClickedCheckShowlog();
	afx_msg void OnInsertitemListctrlLog2(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_ctl_Combo_Engine;
	// 发音人Combo
	CComboBox m_ctl_Combo_Informant;
};
