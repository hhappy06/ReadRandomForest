#include "stdafx.h"
#include "Util.h"
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <stdio.h>
#ifdef WIN32
LARGE_INTEGER Util::m_liFreq;
BOOL Util::m_dummy = QueryPerformanceFrequency(&(Util::m_liFreq));
#endif

Util::Util(const  char * words)
    : m_words(words)
{
#ifdef WIN32
    QueryPerformanceCounter(&m_liTime1);
#else
    struct timeval tv1;
    gettimeofday(&tv1, 0);
    m_time1 = tv1.tv_sec * 1000000 +tv1.tv_usec;
    m_total = 0;
#endif
}


Util::~Util(void)
{
#ifdef WIN32
	if (m_total == 0)
	{
		QueryPerformanceCounter(&m_liTime2);

		m_total = (double)(m_liTime2.QuadPart - m_liTime1.QuadPart);
		m_total *= (1000/**1000*/);
		m_total /= Util::m_liFreq.QuadPart;
	}
	if (m_words && m_words[0])
	{
		//    TRACE("%s: time = %f\n", m_words, time);
		//cout << ("%s: time = %f\n", m_words, m_total);
	}
	else
	{
		//     TRACE("time = %f\n", time);
		//cout << ("time = %f\n", m_total);
	}
#else

	struct timeval tv2;
	gettimeofday(&tv2, 0);
	double time2 = tv2.tv_sec * 1000000 +tv2.tv_usec;
	m_total = (time2 - m_time1) / 1000.0d;

#endif
}

double Util::time()
{
#ifndef WIN32
	struct timeval tv2;
	gettimeofday(&tv2, 0);
	double time2 = tv2.tv_sec * 1000000 +tv2.tv_usec;
	m_total = (time2 - m_time1) / 1000.0d;
	return m_total;
#else
	QueryPerformanceCounter(&m_liTime2);

	m_total = (double)(m_liTime2.QuadPart - m_liTime1.QuadPart);
	m_total *= (1000/**1000*/);
	m_total /= Util::m_liFreq.QuadPart;
	return m_total;
#endif
}

