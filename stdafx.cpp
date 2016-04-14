
// stdafx.cpp : 只包括标准包含文件的源文件
// SinectDemo.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

//
//#ifdef WIN32
////#include <windows.h>
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
//
//    timeus *= (1000 * 1000); // * 1000 * 1000; //ms, us, ns
//    timeus /= freq;
//#else
//    struct timeval tv1;
//
//    gettimeofday(&tv1, 0);
//    timeus = tv1.tv_sec * 1000000 +tv1.tv_usec;
//#endif
//    return timeus;
//}
