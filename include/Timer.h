// Time.h: interface for the CTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMER_H__17A1A4F0_2243_4EB2_8316_8527CD8CA6DC__INCLUDED_)
#define AFX_TIMER_H__17A1A4F0_2243_4EB2_8316_8527CD8CA6DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTimer  
{
public:
	CTimer();
	virtual ~CTimer();
	public: 
	void   start(); 
	void   end(); 
	float   getTime()   const; 
private: 
	__int64               frequency; //   Timer   Frequency 
	float                   resolution; //   Timer   Resolution 
	unsigned   long   mm_timer_start; //   Multimedia   Timer   Start   Value 
	unsigned   long   mm_timer_elapsed; //   Multimedia   Timer   Elapsed   Time 
	bool     performance_timer; //   Using   The   Performance   Timer? 
	__int64               performance_timer_start; //   Performance   Timer   Start   Value 
	__int64               performance_timer_elapsed; //   Performance   Timer   Elapsed   Time     
	float                   startTime; 
	float                   endTime; 
	float                   passTime; 
};
#endif // !defined(AFX_TIME_H__17A1A4F0_2243_4EB2_8316_8527CD8CA6DC__INCLUDED_)
