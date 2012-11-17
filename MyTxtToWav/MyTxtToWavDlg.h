
// MyTxtToWavDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "../include/Timer.h"
#include "../include/MiniDump.h"

//-------------------------------------------------------------------------------
//�������¶���msc.dll �ĵ���������Ϊ�˿�����һ���߳�

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
static float	fTimeOfAll;		//TTS����Ҫ����ʱ��

static BOOL  bRun;
static int	 iFenGeShu = 2000;//2000;//�ָ�Ĵ�С ������������******************************************************
							//��������ļ����ϴ��ı�Ĭ�ϳ�����  1024 ��ô������ 500 �㺺��
							//����ϴ��ĳ��ȿ��Ե� 4096 ��ôҲ������Ӧ�ĵ����� 2000 �㺺��

static	const int  iAll = 128;//�������ķָ��������Էֳ�128�飬��Ȼ�ֳ�120��Ͳ����
static	const int  iAllLength = 4096;


struct m_StrOut				//��������ָ��ַ�������Ϣ�Ľṹ��
{
	CString		strOut;		//������ַ���
	int			iNextStart; //��һ����ʼ��λ��
	int			iStartPot;	//���������ַ����Ŀ�ʼλ��
	int			iLength;	//���������ַ����Ľ���λ��
	//��������������������������鱣�棬��Ϊ��һ�� TTS ʧ�ܣ��´����������ġ�
};

struct m_NeedAgain		//���ڴ洢TTSʧ�ܵ��ַ�����Ϣ����������ٴ�TTS
{
	int		iStart;
	int		iLength;
};


										//����ļ��� 512 ֻ������ ���� һ�� ÿ��txt�����������
										//���� 512 �� ÿ�������� 4096 ����� ������ 512 * 2000 = 1024000
										//��Ȼ�ҳ��Ĳ�����һ��ȥ TTS ��ô�����֣���ĺ�Ҫʱ�����
						
										//charTxt[128][4096]  intTxt_start[512];   intTxt_end[512];
						
										//Ĭ�ϻ��ǲ�����ô��ģ�charTxt[128][4096]  �ͺ��ˣ������ϴ����Ȼ����������һ��������

static char charTxt[iAll][iAllLength];	//�ָ���ַ������  ������� 4096 ������ 1024 �μ� iFenGeShu ��˵��

static m_NeedAgain m_needAgain[iAll];  //���ڴ洢TTSʧ�ܵ��ַ�����Ϣ����������ٴ�TTS



// CMyTxtToWavDlg �Ի���
class CMyTxtToWavDlg : public CDialogEx
{
	//���ݲ���
	struct m_data 
	{
		const char*		str_MyAppId;
		char*			str_Config;
		int				ret;
	//	const char*		sess_id;	
	//	int				synth_status;
		char*			m_chr_WavPath;//�洢wav��·���ļ��������ϳ�
		char*			m_chr_WavPath_Save;//�洢wav��·������������ D:\ �����ļ���
		char*			m_chr_Txt;

		int				iWhichThread;

		char*			m_chr_TxtFileName;
		char*			m_chr_TxtFileNoHouzuiming;//û�ú�����

		CMyTxtToWavDlg*	m_MyTxtToWav;
		CListCtrl *		mylistctrl;
		CListCtrl *		mylistctrl_2;

		CButton   *		m_bt_StartTTS;
		CStatic	  *		m_text_ShowNote;

		CComboBox *		m_ctl_Combo_Informant;

		CString	 *		m_str_TxtPart;//�ָ�õ�һ����
		CString  *		m_str_TxtAll;//ȫ�����ַ���
		
	};
	//һ�δ�2����ȥ���Ͳ�����Ϊָ������
	struct m_data_ForAll
	{
		m_data m_d_1;
		m_data m_d_2;
	};

// ����
public:
	CMyTxtToWavDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMyTxtToWavDlg();

	//----------------------------------------------------
private:
	char *chrtmep;
	void My_DelLog();
public:							

	m_data				m_d_1, m_d_2;				//���ݲ���
	m_data_ForAll		m_data_forall;

	const char* str_MyAppId;//APPID��������Ķ�
	char* str_Config;		//����
	int ret;				//���紫�䷵��ֵ
	CString		m_Str_TxtTemp;//��ʱ����TXT��ȫ������





/*
	MSP_TTS_FLAG_STILL_HAVE_DATA = 1	��Ƶ��ûȡ�꣬���к�̵���Ƶ

	MSP_TTS_FLAG_DATA_END = 2			��Ƶ�Ѿ�ȡ��

	MSP_TTS_FLAG_CMD_CANCELED = 3		����
*/
	//��Ƶ��ûȡ�꣬���к�̵���Ƶ
	int synth_status;
	char* str_Text;

	//MSCΪ��·�Ự������ID������Ψһ�ı�ʶ��·�Ự�����Ժ������������ʱʹ�á���������ʧ����᷵��NULL��
//	char* sess_id;	

public:
	int TTS_Start();	//��ʼTTSʶ��

	static m_StrOut My_FenGe(CString poip, int istart, int ilength);

public:
	static UINT My_thread_TTS(LPVOID lpParam);	//Ĭ��һ���߳̾��ǵ��������

	static UINT My_thread_TTS_2(LPVOID lpParam);//�õ���һ�� DLL��DLL��ͬһ����ֻ����λ�ò�ͬ������ʵ�ֶ��߳�

	static UINT My_thread_All(LPVOID lpParam);


	//----------------------------------------------------
// �Ի�������
	enum { IDD = IDD_MYTXTTOWAV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	// ������Combo
	CComboBox m_ctl_Combo_Informant;
};
