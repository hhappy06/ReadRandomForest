
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
	//    if (m_words && m_words[0])
	//    {
	//        cout << ("%s: time = %f\n", m_words, m_total);
	//    }
	//    else
	//    {
	//        cout << ("time = %f\n", m_total);
	//    }
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

//float CheckFloatSquareDistance(const float fsrc);

//#ifdef WIN32
//LARGE_INTEGER liFreq, liPC1, liPC2;
//int res0 = QueryPerformanceFrequency(&liFreq);
//int res1 = QueryPerformanceCounter(&liPC1);
//#endif
//double gettimeus()
//{
//    double timeus = 0;
//#ifdef WIN32
//    QueryPerformanceCounter(&liPC2);
//    double freq = static_cast<double> (liFreq.QuadPart);
//    timeus = static_cast<double> (liPC2.QuadPart - liPC1.QuadPart);

//    timeus *= (1000 * 1000); // * 1000 * 1000; //ms, us, ns
//    timeus /= freq;
//#else
//    struct timeval tv1;

//    gettimeofday(&tv1, 0);
//    timeus = tv1.tv_sec * 1000000 +tv1.tv_usec;
//#endif
//    return timeus;
//}

//float CheckFloatSquareDistance(const float fsrc)
//{
//    float fdst;
//    float constant = 1.0f;
//    __asm
//    {
//        xorps xmm4, xmm4;
//        movss xmm5, constant;
//        mov     ecx, fsrc;
//        mov     eax, ecx;
//        and     eax, 7F800000h;
//        cmp     eax, 7F800000h;
//        jnz     short loc_1005D6D8;
//        test    ecx, 7FFFFFh;
//        jz      short loc_1005D6D8;
//        mov     dword ptr[fdst], 7FC00000h;
//        jmp     short loc_1005D72F;
//loc_1005D6D8:
//        movss   xmm3, dword ptr [fsrc];
//        ucomiss xmm3, xmm4;
//        lahf;
//        test    ah, 44h;
//        jnp     short loc_1005D71F;
//        comiss  xmm4, xmm3;
//        jbe     short loc_1005D6F6;
//        mov     dword ptr[fdst], 7FFFFFFFh;
//        jmp     short loc_1005D72F;
//loc_1005D6F6:
//        and     ecx, 7FFFFFFFh;
//        cmp     ecx, 7F800000h;
//        jnz     short loc_1005D70C;
//        movss   dword ptr[fdst], xmm4;
//        jmp     short loc_1005D72F;
//loc_1005D70C:
//        sqrtss  xmm3, xmm3;
//        movaps  xmm7, xmm5;
//        divss   xmm7, xmm3;
//        movss   dword ptr[fdst], xmm7;
//        jmp     short loc_1005D72F;
//loc_1005D71F:
//        and     ecx, 0FF800000h;
//        or      ecx, 7F800000h;
//        mov     fdst, ecx;
//loc_1005D72F:
//    }
//    TRACE("float_src = 0x%08X, %f, float_dst = 0x%08X, %f;\n", *((int*)&fsrc), fsrc, *((int*)&fdst), fdst);
//    return fdst;
//}
