#pragma once
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
//#include <QDebug>
#define MEASURE_TIME(X) X
class Util
{
public:
	Util(const  char * = 0);
	~Util();
	double time();
private:
#ifdef WIN32
	static BOOL m_dummy;

	static LARGE_INTEGER m_liFreq;
	LARGE_INTEGER m_liTime1, m_liTime2;
#else
	//struct timeval m_tv1;
	double m_time1;
#endif
	double m_total;
	const  char * m_words;
};




