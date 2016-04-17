#ifndef _BODY_PART_CLASSIFIER_
#define _BODY_PART_CLASSIFIER_
#include <cassert>
#include "NuiSinect.h"
#include "RandomForest.h"
#include "Filter.h"
//using namespace NuiSinect;
//#define _Vector4I_
#ifndef _Vector4I_
#define _Vector4I_
typedef struct _Vector4I
{
    _Vector4I(int ax = 0, int ay = 0, int az = 0)
        : x(ax), y(ay), z(az), w(0)
    {
    }
    int x;
    int y;
    int z;
    int w;
}     Vector4I;
#endif// _Vector4I_

#define _DEPRECATED_
//////////////////////////////////////////////////////////////////////////
#ifndef _SMOOTH_FILTER_STATE_
#define _SMOOTH_FILTER_STATE_
typedef
    enum _SMOOTH_FILTER_STATE
{
    SMOOTH_FILTER_INITIAL  = 0,
    SMOOTH_FILTER_NTRACKED = SMOOTH_FILTER_INITIAL + 1,
    SMOOTH_FILTER_TRACKED  = SMOOTH_FILTER_NTRACKED + 1
}   SMOOTH_FILTER_STATE;
#endif// _SMOOTH_FILTER_STATE_

#ifndef _Joints_Smoothing_State_
#define _Joints_Smoothing_State_
typedef 
struct _Joints_Smoothing_State
{
    _Joints_Smoothing_State()
    {
        memset(&m_Bn, 0, 20 * sizeof(Vector4));
        memset(&m_Xn, 0, 20 * sizeof(Vector4));
        memset(&m_X, 0, 20 * sizeof(Vector4));
        memset(&m_FilterState, 0, 20 * sizeof(SMOOTH_FILTER_STATE));
    }
    Vector4 m_Bn[20];
    Vector4 m_Xn[20];
    Vector4 m_X[20];
    SMOOTH_FILTER_STATE m_FilterState[20];
}   Joints_Smoothing_State;
#endif// _Joints_Smoothing_State_
//////////////////////////////////////////////////////////////////////////



class  BodyPartClassifier
{
public:
    BodyPartClassifier();
	//BodyPartClassifier(cl_context a_context);

	void init();

	~BodyPartClassifier();

    // 加载随机森林分类模型
   // BOOL BuildForestFromFile(const  char * szFileName);
	bool BuildForestFromResource( );

	//private:
    RandomForest     m_forest;
    //unsigned short * m_pLabelRaw;
    //unsigned char  * m_pMask;
    // CentroidsStage_RunFrame用
    // 当前帧所有像素的世界坐标系坐标(x, y, z(depth))
   // Vector4 * m_pvCoordWorldSpace;   //[120][160]
    // 6个人31个部位的概率
   // float   * m_pfBodyPartProbility; // [6][120][160][31]
    int  m_iBodyPartProbilityStep;
    // smooth用
    NUI_TRANSFORM_SMOOTH_PARAMETERS m_defaultSmoothingParams;
    Joints_Smoothing_State          m_msSmoothingState[6];


	//cl_mem cl_m_pfBodyPartProbility;
	//cl_context m_context;

	//@xu-li:过滤器
	Filter *filter;
	Vector4 pretmp1[ 20*6 ];
	Vector4 pretmp2[ 20*6 ];
	Vector4 btmp1[ 20*6 ];
	Vector4 btmp2[ 20*6 ];
	unsigned char isfirstTime[ 20*6 ];

	bool WriteForest(char* pwirtefilename)
	{
		return m_forest.WriteForest(pwirtefilename);
	}

	bool BuildRandomForestFromUnzipfile(char* pfilename)
	{
		return m_forest.BuildRandomForestFromUnzipfile(pfilename);
	}

	bool cmpRandomForest(void)
	{
		return m_forest.cmpRandomForest();
	}
};

#endif//
